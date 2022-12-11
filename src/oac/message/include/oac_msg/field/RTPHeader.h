// Generated by commsdsl2comms v5.0.1

/// @file
/// @brief Contains definition of <b>"RTPHeader"</b> field.

#pragma once

#include <cstdint>
#include <tuple>
#include "comms/field/Bundle.h"
#include "comms/field/IntValue.h"
#include "comms/field/Optional.h"
#include "comms/options.h"
#include "oac_msg/field/FieldBase.h"
#include "oac_msg/field/RTPExtension.h"
#include "oac_msg/field/RTPFlags.h"
#include "oac_msg/field/RTPHeaderCommon.h"
#include "oac_msg/options/DefaultOptions.h"

namespace oac_msg
{

namespace field
{

/// @brief Scope for all the member fields of
///     @ref RTPHeader field.
/// @tparam TOpt Protocol options.
template <typename TOpt = oac_msg::options::DefaultOptions>
struct RTPHeaderMembers
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
            return oac_msg::field::RTPHeaderMembersCommon::FlagsCommon::name();
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
            return oac_msg::field::RTPHeaderMembersCommon::Sequence_numberCommon::hasSpecials();
        }

        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::field::RTPHeaderMembersCommon::Sequence_numberCommon::name();
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
            return oac_msg::field::RTPHeaderMembersCommon::TimestampCommon::hasSpecials();
        }

        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::field::RTPHeaderMembersCommon::TimestampCommon::name();
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
            return oac_msg::field::RTPHeaderMembersCommon::SsrcCommon::hasSpecials();
        }

        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::field::RTPHeaderMembersCommon::SsrcCommon::name();
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
            return oac_msg::field::RTPHeaderMembersCommon::Opt_extensionCommon::name();
        }
    };

    /// @brief All members bundled in @b std::tuple.
    using All =
        std::tuple<
           Flags,
           Sequence_number,
           Timestamp,
           Ssrc,
           Opt_extension
        >;
};

/// @brief Definition of <b>"RTPHeader"</b> field.
/// @tparam TOpt Protocol options.
/// @tparam TExtraOpts Extra options.
template <typename TOpt = oac_msg::options::DefaultOptions, typename... TExtraOpts>
class RTPHeader : public
    comms::field::Bundle<
        oac_msg::field::FieldBase<>,
        typename RTPHeaderMembers<TOpt>::All,
        TExtraOpts...,
        comms::option::def::HasCustomRead,
        comms::option::def::HasCustomRefresh
    >
{
    using Base =
        comms::field::Bundle<
            oac_msg::field::FieldBase<>,
            typename RTPHeaderMembers<TOpt>::All,
            TExtraOpts...,
            comms::option::def::HasCustomRead,
            comms::option::def::HasCustomRefresh
        >;
public:
    /// @brief Allow access to internal fields.
    /// @details See definition of @b COMMS_FIELD_MEMBERS_NAMES macro
    ///     related to @b comms::field::Bundle class from COMMS library
    ///     for details.
    ///
    ///     The generated values, types and access functions are:
    ///     @li @b FieldIdx_flags index, @b Field_flags type and @b field_flags() access function -
    ///         for oac_msg::field::RTPHeaderMembers::Flags member field.
    ///     @li @b FieldIdx_sequence_number index, @b Field_sequence_number type and @b field_sequence_number() access function -
    ///         for oac_msg::field::RTPHeaderMembers::Sequence_number member field.
    ///     @li @b FieldIdx_timestamp index, @b Field_timestamp type and @b field_timestamp() access function -
    ///         for oac_msg::field::RTPHeaderMembers::Timestamp member field.
    ///     @li @b FieldIdx_ssrc index, @b Field_ssrc type and @b field_ssrc() access function -
    ///         for oac_msg::field::RTPHeaderMembers::Ssrc member field.
    ///     @li @b FieldIdx_opt_extension index, @b Field_opt_extension type and @b field_opt_extension() access function -
    ///         for oac_msg::field::RTPHeaderMembers::Opt_extension member field.
    COMMS_FIELD_MEMBERS_NAMES(
        flags,
        sequence_number,
        timestamp,
        ssrc,
        opt_extension
    );

    /// @brief Name of the field.
    static const char* name()
    {
        return oac_msg::field::RTPHeaderCommon::name();
    }

    /// @brief Generated read functionality.
    template <typename TIter>
    comms::ErrorStatus read(TIter& iter, std::size_t len)
    {
        auto es = comms::ErrorStatus::Success;
        do {
            es = Base::template readUntilAndUpdateLen<FieldIdx_opt_extension>(iter, len);
            if (es != comms::ErrorStatus::Success) {
                break;
            }

            readPrepare_opt_extension();

            es = Base::template readFrom<FieldIdx_opt_extension>(iter, len);
        } while(false);
        return es;
    }

    /// @brief Generated refresh functionality.
    bool refresh()
    {
        bool updated = Base::refresh();
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

} // namespace field

} // namespace oac_msg