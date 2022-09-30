// Generated by commsdsl2comms v5.0.1

/// @file
/// @brief Contains definition of protocol data view default options.

#pragma once

#include "oac_msg/options/DefaultOptions.h"

namespace oac_msg
{

namespace options
{

/// @brief Default data view options of the protocol.
template <typename TBase = oac_msg::options::DefaultOptions>
struct DataViewDefaultOptionsT : public TBase
{
    /// @brief Extra options for frames.
    struct frame : public TBase::frame
    {
        /// @brief Extra options for layers of
        ///     @ref oac_msg::frame::Frame frame.
        struct FrameLayers : public TBase::frame::FrameLayers
        {
            /// @brief Extra options for @ref
            ///     oac_msg::frame::FrameLayers::Data layer.
            using Data =
                std::tuple<
                    comms::option::app::OrigDataView,
                    typename TBase::frame::FrameLayers::Data
                >;
        }; // struct FrameLayers
    }; // struct frame
};

/// @brief Alias to @ref DataViewDefaultOptionsT with default template parameter.
using DataViewDefaultOptions = DataViewDefaultOptionsT<>;

} // namespace options

} // namespace oac_msg