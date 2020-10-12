#pragma once
#include <span>
#include <vector>
#include <ezipc/core/types.hpp>

namespace ezipc {

// clang-format off
template<typename Codec_>
concept StreamCodec = requires (Codec_ C, std::vector<byte> Data)
{
    {C.reset_state(typename Codec_::codec_argument_type{})};
    {C.decode(std::span<const byte>{}, Data)};
    {C.encode(std::span<const byte>{}, Data)};
};
// clang-format on

} // namespace ezipc