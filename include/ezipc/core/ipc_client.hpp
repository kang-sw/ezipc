#pragma once
#include <span>
#include <chrono>
#include <ezipc/core/impl/__ipc_client.hpp>
#include <ezipc/core/interfaces.hxx>
#include <ezipc/core/types.hpp>

namespace ezipc {

// clang-format off
template <class Conn_>
concept ClientConnector = requires(Conn_ C, std::vector<byte> Data)
{
    {C.try_connect(typename Conn_::connection_argument_type{})}->std::convertible_to<error>;
    {C.write(std::span<byte>{})}->std::convertible_to<error>;
    {C.read(Data, std::chrono::milliseconds {})}->std::convertible_to<error>;
    {C.try_dispose()}->std::convertible_to<error>;
};
// clang-format on

struct default_stream_codec {
    struct codec_argument_type { };
    void reset_state(codec_argument_type) const { }
    void decode(std::span<const byte> encoded, std::vector<byte>& data) { data.assign(encoded.begin(), encoded.end()); }
    void encode(std::span<const byte> data, std::vector<byte>& encoded) { encoded.assign(data.begin(), data.end()); }
};

template <
  ClientConnector ReliableConn_,
  ClientConnector UnreiliableConn_,
  StreamCodec Codec_ = default_stream_codec,
  class Alloc_ = std::allocator<byte>>
class ipc_client
    : internal::ipc_client_base
    , public std::enable_shared_from_this<ipc_client<ReliableConn_, UnreiliableConn_, Codec_, Alloc_>>
{
public:
    using reliable_connection_argument_type = typename ReliableConn_::connection_argument_type;
    using unreliable_connection_argument_type = typename UnreiliableConn_::connection_argument_type;
    using codec_argument_type = typename Codec_::codec_argument_type;

public:
    friend std::shared_ptr<ipc_client> std::make_shared();
};
} // namespace ezipc
