#pragma once
#include <bitset>

namespace ezipc {

enum class sync_policy {
    request,
    lazy,
    poll
};

using var_id_t = uint64_t;
static var_id_t var_id_none = static_cast<var_id_t>(-1);

enum class error { okay };

using byte = std::byte;

} // namespace ezipc
