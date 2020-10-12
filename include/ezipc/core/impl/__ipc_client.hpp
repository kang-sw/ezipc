#pragma once
#include <span>
#include <functional>
#include <ezipc/core/types.hpp>

namespace ezipc::internal {

class ipc_client_base
{
private:
    // upload functionality

    // download functionality

private: // 포인터 관련
    friend class ipc_ptr_base;

    // pointer creation & dispose
    // updates internal sync policy and upload changes on sync policies to server
    void incr_refcnt(var_id_t id, sync_policy);
    void decr_refcnt(var_id_t id, intptr_t ptr, sync_policy);

    // write operation shares all policies
    void write(var_id_t id, size_t offset, std::span<char const> value);

    // policy: lazy, request
    void read_request_async(var_id_t id, size_t offset, std::function<void(std::span<char const>, error)>&&);

    // policy: poll
    void register_notify_handler(var_id_t id, intptr_t ptr, std::function<void(std::span<char const>, error)>&&); // for poll policy
    void read(var_id_t id, size_t offset, std::span<char> buffer);
    bool can_read(var_id_t id, size_t offset, std::span<char> buffer) const; // 메모리를 즉시 읽을수 있는지 검사

    // returns var_id_none if given key already exists
    var_id_t allocate(std::string_view key, sync_policy);

    // returns var_id_none if there's no allocation updated
    var_id_t watch(std::string_view key, sync_policy);

private:
protected:
    ipc_client_base() = default;
    virtual ~ipc_client_base() = default;
};
} // namespace ezipc::internal