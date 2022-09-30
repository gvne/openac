// Generated by commsdsl2comms v5.0.1

/// @file
/// @brief Contains definition of <b>"DNTPMessage"</b> message and its fields.

#pragma once

#include <cstdint>
#include <tuple>
#include "comms/MessageBase.h"
#include "comms/field/IntValue.h"
#include "comms/options.h"
#include "oac_msg/MsgId.h"
#include "oac_msg/field/FieldBase.h"
#include "oac_msg/field/Timestamp.h"
#include "oac_msg/message/DNTPMessageCommon.h"
#include "oac_msg/options/DefaultOptions.h"

namespace oac_msg
{

namespace message
{

/// @brief Fields of @ref DNTPMessage.
/// @tparam TOpt Extra options
/// @see @ref DNTPMessage
/// @headerfile oac_msg/message/DNTPMessage.h
template <typename TOpt = oac_msg::options::DefaultOptions>
struct DNTPMessageFields
{
    /// @brief Definition of <b>"version"</b> field.
    class Version : public
        comms::field::IntValue<
            oac_msg::field::FieldBase<>,
            std::uint8_t,
            comms::option::def::DefaultNumValue<1>
        >
    {
        using Base =
            comms::field::IntValue<
                oac_msg::field::FieldBase<>,
                std::uint8_t,
                comms::option::def::DefaultNumValue<1>
            >;
    public:
        /// @brief Re-definition of the value type.
        using ValueType = typename Base::ValueType;

        /// @brief Compile time detection of special values presence.
        static constexpr bool hasSpecials()
        {
            return oac_msg::message::DNTPMessageFieldsCommon::VersionCommon::hasSpecials();
        }

        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::message::DNTPMessageFieldsCommon::VersionCommon::name();
        }
    };

    /// @brief Definition of <b>"originate_timestamp"</b> field.
    class Originate_timestamp : public
        oac_msg::field::Timestamp<
            TOpt
        >
    {
        using Base =
            oac_msg::field::Timestamp<
                TOpt
            >;
    public:
        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::message::DNTPMessageFieldsCommon::Originate_timestampCommon::name();
        }
    };

    /// @brief Definition of <b>"receive_timestamp"</b> field.
    class Receive_timestamp : public
        oac_msg::field::Timestamp<
            TOpt
        >
    {
        using Base =
            oac_msg::field::Timestamp<
                TOpt
            >;
    public:
        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::message::DNTPMessageFieldsCommon::Receive_timestampCommon::name();
        }
    };

    /// @brief Definition of <b>"transmit_timestamp"</b> field.
    class Transmit_timestamp : public
        oac_msg::field::Timestamp<
            TOpt
        >
    {
        using Base =
            oac_msg::field::Timestamp<
                TOpt
            >;
    public:
        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::message::DNTPMessageFieldsCommon::Transmit_timestampCommon::name();
        }
    };

    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        Version,
        Originate_timestamp,
        Receive_timestamp,
        Transmit_timestamp
    >;
};

/// @brief Definition of <b>"DNTPMessage"</b> message class.
/// @details
///     See @ref DNTPMessageFields for definition of the fields this message contains.
/// @tparam TMsgBase Base (interface) class.
/// @tparam TOpt Extra options
/// @headerfile oac_msg/message/DNTPMessage.h
template <typename TMsgBase, typename TOpt = oac_msg::options::DefaultOptions>
class DNTPMessage : public
    comms::MessageBase<
        TMsgBase,
        comms::option::def::StaticNumIdImpl<oac_msg::MsgId_DNTPMessage>,
        comms::option::def::FieldsImpl<typename DNTPMessageFields<TOpt>::All>,
        comms::option::def::MsgType<DNTPMessage<TMsgBase, TOpt> >,
        comms::option::def::HasName
    >
{
    // Redefinition of the base class type
    using Base =
        comms::MessageBase<
            TMsgBase,
            comms::option::def::StaticNumIdImpl<oac_msg::MsgId_DNTPMessage>,
            comms::option::def::FieldsImpl<typename DNTPMessageFields<TOpt>::All>,
            comms::option::def::MsgType<DNTPMessage<TMsgBase, TOpt> >,
            comms::option::def::HasName
        >;

public:
    /// @brief Provide names and allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    ///
    ///     The generated values, types and functions are:
    ///     @li @b FieldIdx_version index, @b Field_version type and @b field_version() access fuction
    ///         for @ref DNTPMessageFields::Version field.
    ///     @li @b FieldIdx_originate_timestamp index, @b Field_originate_timestamp type and @b field_originate_timestamp() access fuction
    ///         for @ref DNTPMessageFields::Originate_timestamp field.
    ///     @li @b FieldIdx_receive_timestamp index, @b Field_receive_timestamp type and @b field_receive_timestamp() access fuction
    ///         for @ref DNTPMessageFields::Receive_timestamp field.
    ///     @li @b FieldIdx_transmit_timestamp index, @b Field_transmit_timestamp type and @b field_transmit_timestamp() access fuction
    ///         for @ref DNTPMessageFields::Transmit_timestamp field.
    COMMS_MSG_FIELDS_NAMES(
        version,
        originate_timestamp,
        receive_timestamp,
        transmit_timestamp
    );

    // Compile time check for serialisation length.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static const std::size_t MsgMaxLen = Base::doMaxLength();
    static_assert(MsgMinLen == 25U, "Unexpected min serialisation length");
    static_assert(MsgMaxLen == 25U, "Unexpected max serialisation length");

    /// @brief Name of the message.
    static const char* doName()
    {
        return oac_msg::message::DNTPMessageCommon::name();
    }
};

} // namespace message

} // namespace oac_msg