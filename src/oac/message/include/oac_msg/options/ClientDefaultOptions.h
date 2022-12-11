// Generated by commsdsl2comms v5.0.1

/// @file
/// @brief Contains definition of protocol client default options.

#pragma once

#include "oac_msg/options/DefaultOptions.h"

namespace oac_msg
{

namespace options
{

/// @brief Default client options of the protocol.
template <typename TBase = oac_msg::options::DefaultOptions>
struct ClientDefaultOptionsT : public TBase
{
};

/// @brief Alias to @ref ClientDefaultOptionsT with default template parameter.
using ClientDefaultOptions = ClientDefaultOptionsT<>;

} // namespace options

} // namespace oac_msg
