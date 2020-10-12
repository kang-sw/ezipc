#pragma once
#include <iostream>
#include <functional>

namespace ezipc {

enum logger_verbosity : int
{
    logger_verbosity_none,

    logger_verbosity_verbose,
    logger_verbosity_debug,
    logger_verbosity_info,
    logger_verbosity_warning,
    logger_verbosity_error,
    logger_verbosity_fatal,

    logger_verbosity_always = std::numeric_limits<int>::max()
};

#ifndef SICO_COMPILE_TIME_MUTED_VERBOSITY
/**
 * 컴파일 시간에 최적화할 로그 verbosity를 조정합니다. 
 */
#define SICO_COMPILE_TIME_MUTED_VERBOSITY logger_verbosity_none
#endif

/**
 * 전역 verbosity 값입니다. 이 값보다 큰 로그 verbosity를 가진 로거만 출력됩니다.
 * 기본값은 debug로 초기화되며, 런타임에 변경할 수 있습니다.
 */
extern logger_verbosity global_logger_muted_verbosity;

/**
 * 기본 pre_logger 메서드입니다.
 * verbosity와 시간을 출력합니다.
 */
void default_pre_logger(logger_verbosity, std::ostream&);

/** 
 * 로깅을 위한 클래스입니다. 런타임, 또는 컴파일 타임에 로그 활성화 여부를 선택 가능합니다.
 * ostream을 상속하는 스트림을 참조합니다.
 * 컴파일 시간에 사용되지 않는 logger가 효율적으로 최적화되도록, << 연산자를 오버로드하지 않습니다.
 */
template <
  logger_verbosity compile_time_verbosity = logger_verbosity_always,
  typename alloc_type                     = std::allocator<char>>
class logger
{
public:
    using allocator_type = std::allocator<char>;

public:
    enum
    {
        static_verbosity = compile_time_verbosity
    };

public:
    // 매 로깅 전마다 호출되는 함수입니다.
    std::function<void(logger_verbosity, std::ostream&)> pre_logger        = default_pre_logger;
    logger_verbosity                                     dynamic_verbosity = compile_time_verbosity;

public:
    logger() noexcept {};
    logger(std::ostream* pstream) noexcept
        : strm_(pstream){};

    ~logger()
    {
        reset();
    }

    // Non-copyable
    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;

    /**
     * 단순 레퍼런스를 재지정합니다.
     * @param pstream 재지정할 레퍼런스입니다. 이 로거를 동적으로 무효화하려는 경우 널 지정
     * @note 매개 변수로 전달된 pstream 레퍼런스는 logger보다 먼저 무효화되어서는 안 됩니다
     */
    void reset(std::ostream* pstream = nullptr)
    {
        // Explicitly destroy managed object
        if (alloc_size_)
        {
            strm_->~basic_ostream();
            allocator_type().deallocate(reinterpret_cast<char*>(strm_), alloc_size_);
            alloc_size_ = 0;
        }

        strm_ = pstream;
    }

    /**
     * 이 로거 내부에 스트림을 생성합니다.
     * 이렇게 생성된 스트림은 관리되며, logger 파괴 시 자동으로 파괴됩니다.
     */
    template <typename stream_type,
              typename... cstr_args>
    void emplace_stream(cstr_args&&... args)
    {
        reset();
        alloc_size_ = sizeof(stream_type);
        strm_       = reinterpret_cast<std::ostream*>(allocator_type().allocate(alloc_size_));
        new (strm_) stream_type(std::forward<cstr_args>(args)...);
    }

    /**
     * 모든 인자를 출력합니다.
     */
    template <typename arg_t, typename... args_t>
    void operator()(arg_t&& arg, args_t&&... args)
    {
        if constexpr (static_verbosity > SICO_COMPILE_TIME_MUTED_VERBOSITY)
        {
            if (dynamic_verbosity <= global_logger_muted_verbosity)
                return;

            if (strm_ == nullptr)
                return;

            if (pre_logger)
                pre_logger(dynamic_verbosity, *strm_);

            *strm_ << std::forward<arg_t>(arg);
            ((*strm_ << std::forward<args_t>(args)), ...);
            *strm_ << '\n';
        }
    }

    /**
     * 모든 인자를 단순 출력합니다.
     * 단, pre_logger와 개행 문자가 삽입되지 않습니다.
     */
    template <typename arg_t, typename... args_t>
    void put(arg_t&& arg, args_t&&... args)
    {
        if constexpr (static_verbosity > SICO_COMPILE_TIME_MUTED_VERBOSITY)
        {
            if (dynamic_verbosity <= global_logger_muted_verbosity)
                return;

            if (strm_ == nullptr)
                return;

            *strm_ << std::forward<arg_t>(arg);
            ((*strm_ << std::forward<args_t>(args)), ...);
        }
    }

    /**
     * pre_logger를 강제 호출합니다.
     */
    void pre_log()
    {
        pre_logger(dynamic_verbosity, *strm_);
    }

private:
    std::ostream* strm_       = nullptr;
    size_t        alloc_size_ = 0;
};

// Global loggers
extern logger<logger_verbosity_verbose> log_verbose;
extern logger<logger_verbosity_debug>   log_debug;
extern logger<logger_verbosity_info>    log_info;
extern logger<logger_verbosity_warning> log_warning;
extern logger<logger_verbosity_error>   log_error;
extern logger<logger_verbosity_fatal>   log_fatal;

} // namespace sico