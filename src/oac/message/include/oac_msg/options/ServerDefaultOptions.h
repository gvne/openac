// Generated by commsdsl2comms v5.0.1

/// @file
/// @brief Contains definition of protocol server default options.

#pragma once

#include "oac_msg/options/DefaultOptions.h"

namespace oac_msg
{

namespace options
{

/// @brief Default server options of the protocol.
template <typename TBase = oac_msg::options::DefaultOptions>
struct ServerDefaultOptionsT : public TBase
{
};

/// @brief Alias to @ref ServerDefaultOptionsT with default template parameter.
using ServerDefaultOptions = ServerDefaultOptionsT<>;

} // namespace options

} // namespace oac_msg
