#include <ezipc/class/log.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

namespace ezipc {
logger_verbosity                 global_logger_muted_verbosity = logger_verbosity_debug;
logger<logger_verbosity_verbose> log_verbose(&std::cout);
logger<logger_verbosity_debug>   log_debug(&std::cout);
logger<logger_verbosity_info>    log_info(&std::cout);
logger<logger_verbosity_warning> log_warning(&std::cout);
logger<logger_verbosity_error>   log_error(&std::cout);
logger<logger_verbosity_fatal>   log_fatal(&std::cout);

void default_pre_logger(logger_verbosity v, std::ostream& strm)
{
    char const* tags[] = {
      "none",
      "verbose",
      "debug",
      "info",
      "warning",
      "error",
      "fatal"};

    const char* tag = "always";

    if (v >= 0 && v <= logger_verbosity_fatal)
        tag = tags[v];

    auto now  = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);

    tm t;
    localtime_s(&t, &time);

    strm << put_time(&t, "[%H:%M:%S | ") << tag << "] ";
}
} // namespace sico