// Generated by commsdsl2comms v5.0.1

/// @file
/// @brief Contains definition of <b>"Timestamp"</b> field.

#pragma once

#include <cstdint>
#include <tuple>
#include "comms/field/Bundle.h"
#include "comms/field/IntValue.h"
#include "comms/options.h"
#include "oac_msg/field/FieldBase.h"
#include "oac_msg/field/TimestampCommon.h"
#include "oac_msg/options/DefaultOptions.h"

namespace oac_msg
{

namespace field
{

/// @brief Scope for all the member fields of
///     @ref Timestamp field.
/// @tparam TOpt Protocol options.
template <typename TOpt = oac_msg::options::DefaultOptions>
struct TimestampMembers
{
    /// @brief Definition of <b>"second"</b> field.
    class Second : public
        comms::field::IntValue<
            oac_msg::field::FieldBase<>,
            std::int32_t
        >
    {
        using Base =
            comms::field::IntValue<
                oac_msg::field::FieldBase<>,
                std::int32_t
            >;
    public:
        /// @brief Re-definition of the value type.
        using ValueType = typename Base::ValueType;

        /// @brief Compile time detection of special values presence.
        static constexpr bool hasSpecials()
        {
            return oac_msg::field::TimestampMembersCommon::SecondCommon::hasSpecials();
        }

        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::field::TimestampMembersCommon::SecondCommon::name();
        }
    };

    /// @brief Definition of <b>"fraction"</b> field.
    class Fraction : public
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
            return oac_msg::field::TimestampMembersCommon::FractionCommon::hasSpecials();
        }

        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::field::TimestampMembersCommon::FractionCommon::name();
        }
    };

    /// @brief All members bundled in @b std::tuple.
    using All =
        std::tuple<
           Second,
           Fraction
        >;
};

/// @brief Definition of <b>"Timestamp"</b> field.
/// @tparam TOpt Protocol options.
/// @tparam TExtraOpts Extra options.
template <typename TOpt = oac_msg::options::DefaultOptions, typename... TExtraOpts>
class Timestamp : public
    comms::field::Bundle<
        oac_msg::field::FieldBase<>,
        typename TimestampMembers<TOpt>::All,
        TExtraOpts...
    >
{
    using Base =
        comms::field::Bundle<
            oac_msg::field::FieldBase<>,
            typename TimestampMembers<TOpt>::All,
            TExtraOpts...
        >;
public:
    /// @brief Allow access to internal fields.
    /// @details See definition of @b COMMS_FIELD_MEMBERS_NAMES macro
    ///     related to @b comms::field::Bundle class from COMMS library
    ///     for details.
    ///
    ///     The generated values, types and access functions are:
    ///     @li @b FieldIdx_second index, @b Field_second type and @b field_second() access function -
    ///         for oac_msg::field::TimestampMembers::Second member field.
    ///     @li @b FieldIdx_fraction index, @b Field_fraction type and @b field_fraction() access function -
    ///         for oac_msg::field::TimestampMembers::Fraction member field.
    COMMS_FIELD_MEMBERS_NAMES(
        second,
        fraction
    );

    /// @brief Name of the field.
    static const char* name()
    {
        return oac_msg::field::TimestampCommon::name();
    }
};

} // namespace field

} // namespace oac_msg
