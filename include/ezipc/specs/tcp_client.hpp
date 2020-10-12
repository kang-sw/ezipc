#pragma once
#include <ezipc/core/ipc_client.hpp>

namespace ezipc::tcp {

class client_connector
{
public:
    struct connection_argument_type { };

public:
    error try_connect(connection_argument_type const&);
    error write(std::span<byte>);
    error read(std::vector<byte>&, std::chrono::milliseconds timeout);
    error try_dispose();
};

} // namespace ezipc::tcp
