// Generated by commsdsl2comms v5.0.1

/// @file
/// @brief Contains definition of protocol default options.

#pragma once

#include "comms/options.h"

namespace oac_msg
{

namespace options
{

/// @brief Empty class to serve as the base for options.
struct EmptyOptions {};

/// @brief Default (empty) options of the protocol.
template <typename TBase = EmptyOptions>
struct DefaultOptionsT : public TBase
{
    /// @brief Extra options for fields.
    struct field
    {
        struct RTPExtensionMembers
        {
            struct GenericMembers
            {
                /// @brief Extra options for @ref
                ///     oac_msg::field::RTPExtensionMembers::GenericMembers::Content
                ///     field.
                using Content = comms::option::EmptyOption;
            }; // struct GenericMembers

            struct Oac_extensionMembers
            {
                /// @brief Extra options for @ref
                ///     oac_msg::field::RTPExtensionMembers::Oac_extensionMembers::Dntp_server_ipv4_address
                ///     field.
                using Dntp_server_ipv4_address = comms::option::EmptyOption;
            }; // struct Oac_extensionMembers
        }; // struct RTPExtensionMembers
    }; // struct field

    /// @brief Extra options for messages.
    struct message
    {
        /// @brief Extra options for fields of
        ///     @ref oac_msg::message::RTPMessage message.
        struct RTPMessageFields
        {
            /// @brief Extra options for @ref
            ///     oac_msg::message::RTPMessageFields::Content
            ///     field.
            using Content = comms::option::EmptyOption;
        };
    }; // struct message

    /// @brief Extra options for frames.
    struct frame
    {
        /// @brief Extra options for layers of
        ///     @ref oac_msg::frame::Frame frame.
        struct FrameLayers
        {
            /// @brief Extra options for @ref
            ///     oac_msg::frame::FrameLayers::Data layer.
            using Data = comms::option::EmptyOption;
        }; // struct FrameLayers
    }; // struct frame
};

/// @brief Default (empty) options of the protocol.
using DefaultOptions = DefaultOptionsT<>;

} // namespace options

} // namespace oac_msg
