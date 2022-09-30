// Generated by commsdsl2comms v5.0.1

/// @file
/// @brief Contains definition of <b>"RTPMessage"</b> message and its fields.

#pragma once

#include <cstdint>
#include <tuple>
#include "comms/MessageBase.h"
#include "comms/field/ArrayList.h"
#include "comms/field/IntValue.h"
#include "comms/field/Optional.h"
#include "comms/options.h"
#include "oac_msg/MsgId.h"
#include "oac_msg/field/FieldBase.h"
#include "oac_msg/field/RTPExtension.h"
#include "oac_msg/field/RTPFlags.h"
#include "oac_msg/message/RTPMessageCommon.h"
#include "oac_msg/options/DefaultOptions.h"

namespace oac_msg
{

namespace message
{

/// @brief Fields of @ref RTPMessage.
/// @tparam TOpt Extra options
/// @see @ref RTPMessage
/// @headerfile oac_msg/message/RTPMessage.h
template <typename TOpt = oac_msg::options::DefaultOptions>
struct RTPMessageFields
{
    /// @brief Definition of <b>"flags"</b> field.
    class Flags : public
        oac_msg::field::RTPFlags<
            TOpt
        >
    {
        using Base =
            oac_msg::field::RTPFlags<
                TOpt
            >;
    public:
        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::message::RTPMessageFieldsCommon::FlagsCommon::name();
        }
    };

    /// @brief Definition of <b>"sequence_number"</b> field.
    class Sequence_number : public
        comms::field::IntValue<
            oac_msg::field::FieldBase<>,
            std::uint16_t
        >
    {
        using Base =
            comms::field::IntValue<
                oac_msg::field::FieldBase<>,
                std::uint16_t
            >;
    public:
        /// @brief Re-definition of the value type.
        using ValueType = typename Base::ValueType;

        /// @brief Compile time detection of special values presence.
        static constexpr bool hasSpecials()
        {
            return oac_msg::message::RTPMessageFieldsCommon::Sequence_numberCommon::hasSpecials();
        }

        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::message::RTPMessageFieldsCommon::Sequence_numberCommon::name();
        }
    };

    /// @brief Definition of <b>"timestamp"</b> field.
    class Timestamp : public
        comms::field::IntValue<
            oac_msg::field::FieldBase<>,
            std::uint32_t
        >
    {
        using Base =
            comms::field::IntValue<
                oac_msg::field::FieldBase<>,
                std::uint32_t
            >;
    public:
        /// @brief Re-definition of the value type.
        using ValueType = typename Base::ValueType;

        /// @brief Compile time detection of special values presence.
        static constexpr bool hasSpecials()
        {
            return oac_msg::message::RTPMessageFieldsCommon::TimestampCommon::hasSpecials();
        }

        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::message::RTPMessageFieldsCommon::TimestampCommon::name();
        }
    };

    /// @brief Definition of <b>"ssrc"</b> field.
    class Ssrc : public
        comms::field::IntValue<
            oac_msg::field::FieldBase<>,
            std::uint32_t
        >
    {
        using Base =
            comms::field::IntValue<
                oac_msg::field::FieldBase<>,
                std::uint32_t
            >;
    public:
        /// @brief Re-definition of the value type.
        using ValueType = typename Base::ValueType;

        /// @brief Compile time detection of special values presence.
        static constexpr bool hasSpecials()
        {
            return oac_msg::message::RTPMessageFieldsCommon::SsrcCommon::hasSpecials();
        }

        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::message::RTPMessageFieldsCommon::SsrcCommon::name();
        }
    };

    /// @brief Scope for all the member fields of
    ///     @ref Opt_extension field.
    struct Opt_extensionMembers
    {
        /// @brief Definition of <b>"RTPExtension"</b> field.
        using RTPExtension =
            oac_msg::field::RTPExtension<
                TOpt
            >;
    };

    /// @brief Definition of <b>"opt_extension"</b> field.
    class Opt_extension : public
        comms::field::Optional<
            typename Opt_extensionMembers::RTPExtension
        >
    {
        using Base =
            comms::field::Optional<
                typename Opt_extensionMembers::RTPExtension
            >;
    public:
        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::message::RTPMessageFieldsCommon::Opt_extensionCommon::name();
        }
    };

    /// @brief Scope for all the member fields of
    ///     @ref Content field.
    struct ContentMembers
    {
        /// @brief Definition of <b>"element"</b> field.
        class Element : public
            comms::field::IntValue<
                oac_msg::field::FieldBase<>,
                std::uint8_t
            >
        {
            using Base =
                comms::field::IntValue<
                    oac_msg::field::FieldBase<>,
                    std::uint8_t
                >;
        public:
            /// @brief Re-definition of the value type.
            using ValueType = typename Base::ValueType;

            /// @brief Compile time detection of special values presence.
            static constexpr bool hasSpecials()
            {
                return oac_msg::message::RTPMessageFieldsCommon::ContentMembersCommon::ElementCommon::hasSpecials();
            }

            /// @brief Name of the field.
            static const char* name()
            {
                return oac_msg::message::RTPMessageFieldsCommon::ContentMembersCommon::ElementCommon::name();
            }
        };
    };

    /// @brief Definition of <b>"content"</b> field.
    class Content : public
        comms::field::ArrayList<
            oac_msg::field::FieldBase<>,
            typename ContentMembers::Element,
            typename TOpt::message::RTPMessageFields::Content
        >
    {
        using Base =
            comms::field::ArrayList<
                oac_msg::field::FieldBase<>,
                typename ContentMembers::Element,
                typename TOpt::message::RTPMessageFields::Content
            >;
    public:
        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::message::RTPMessageFieldsCommon::ContentCommon::name();
        }
    };

    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        Flags,
        Sequence_number,
        Timestamp,
        Ssrc,
        Opt_extension,
        Content
    >;
};

/// @brief Definition of <b>"RTPMessage"</b> message class.
/// @details
///     See @ref RTPMessageFields for definition of the fields this message contains.
/// @tparam TMsgBase Base (interface) class.
/// @tparam TOpt Extra options
/// @headerfile oac_msg/message/RTPMessage.h
template <typename TMsgBase, typename TOpt = oac_msg::options::DefaultOptions>
class RTPMessage : public
    comms::MessageBase<
        TMsgBase,
        comms::option::def::StaticNumIdImpl<oac_msg::MsgId_RTPMessage>,
        comms::option::def::FieldsImpl<typename RTPMessageFields<TOpt>::All>,
        comms::option::def::MsgType<RTPMessage<TMsgBase, TOpt> >,
        comms::option::def::HasName,
        comms::option::def::HasCustomRefresh
    >
{
    // Redefinition of the base class type
    using Base =
        comms::MessageBase<
            TMsgBase,
            comms::option::def::StaticNumIdImpl<oac_msg::MsgId_RTPMessage>,
            comms::option::def::FieldsImpl<typename RTPMessageFields<TOpt>::All>,
            comms::option::def::MsgType<RTPMessage<TMsgBase, TOpt> >,
            comms::option::def::HasName,
            comms::option::def::HasCustomRefresh
        >;

public:
    /// @brief Provide names and allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    ///
    ///     The generated values, types and functions are:
    ///     @li @b FieldIdx_flags index, @b Field_flags type and @b field_flags() access fuction
    ///         for @ref RTPMessageFields::Flags field.
    ///     @li @b FieldIdx_sequence_number index, @b Field_sequence_number type and @b field_sequence_number() access fuction
    ///         for @ref RTPMessageFields::Sequence_number field.
    ///     @li @b FieldIdx_timestamp index, @b Field_timestamp type and @b field_timestamp() access fuction
    ///         for @ref RTPMessageFields::Timestamp field.
    ///     @li @b FieldIdx_ssrc index, @b Field_ssrc type and @b field_ssrc() access fuction
    ///         for @ref RTPMessageFields::Ssrc field.
    ///     @li @b FieldIdx_opt_extension index, @b Field_opt_extension type and @b field_opt_extension() access fuction
    ///         for @ref RTPMessageFields::Opt_extension field.
    ///     @li @b FieldIdx_content index, @b Field_content type and @b field_content() access fuction
    ///         for @ref RTPMessageFields::Content field.
    COMMS_MSG_FIELDS_NAMES(
        flags,
        sequence_number,
        timestamp,
        ssrc,
        opt_extension,
        content
    );

    // Compile time check for serialisation length.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static_assert(MsgMinLen == 12U, "Unexpected min serialisation length");

    /// @brief Name of the message.
    static const char* doName()
    {
        return oac_msg::message::RTPMessageCommon::name();
    }

    /// @brief Generated read functionality.
    template <typename TIter>
    comms::ErrorStatus doRead(TIter& iter, std::size_t len)
    {
        auto es = comms::ErrorStatus::Success;
        do {
            es = Base::template doReadUntilAndUpdateLen<FieldIdx_opt_extension>(iter, len);
            if (es != comms::ErrorStatus::Success) {
                break;
            }

            readPrepare_opt_extension();

            es = Base::template doReadFrom<FieldIdx_opt_extension>(iter, len);
        } while (false);
        return es;
    }

    /// @brief Generated refresh functionality.
    bool doRefresh()
    {
       bool updated = Base::doRefresh();
       updated = refresh_opt_extension() || updated;
       return updated;
    }

private:
    void readPrepare_opt_extension()
    {
        refresh_opt_extension();
    }

    bool refresh_opt_extension()
    {
        auto mode = comms::field::OptionalMode::Missing;
        if (field_flags().field_extension().getValue() == static_cast<typename Field_flags::Field_extension::ValueType>(1U)) {
            mode = comms::field::OptionalMode::Exists;
        }

        if (field_opt_extension().getMode() == mode) {
            return false;
        }

        field_opt_extension().setMode(mode);
        return true;
    }
};

} // namespace message

} // namespace oac_msg
