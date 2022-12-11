// Generated by commsdsl2comms v5.0.1

/// @file
/// @brief Contains common template parameters independent functionality of
///    @ref oac_msg::field::Timestamp field.

#pragma once

#include <cstdint>

namespace oac_msg
{

namespace field
{

/// @brief Scope for all the common definitions of the member fields of
///     @ref oac_msg::field::Timestamp field.
struct TimestampMembersCommon
{
    /// @brief Common types and functions for
    ///     @ref oac_msg::field::TimestampMembers::Second field.
    struct SecondCommon
    {
        /// @brief Re-definition of the value type used by
        ///     oac_msg::field::TimestampMembers::Second field.
        using ValueType = std::int32_t;

        /// @brief Name of the @ref oac_msg::field::TimestampMembers::Second field.
        static const char* name()
        {
            return "second";
        }

        /// @brief Compile time detection of special values presence.
        static constexpr bool hasSpecials()
        {
            return false;
        }
    };

    /// @brief Common types and functions for
    ///     @ref oac_msg::field::TimestampMembers::Fraction field.
    struct FractionCommon
    {
        /// @brief Re-definition of the value type used by
        ///     oac_msg::field::TimestampMembers::Fraction field.
        using ValueType = std::uint32_t;

        /// @brief Name of the @ref oac_msg::field::TimestampMembers::Fraction field.
        static const char* name()
        {
            return "fraction";
        }

        /// @brief Compile time detection of special values presence.
        static constexpr bool hasSpecials()
        {
            return false;
        }
    };
};

/// @brief Common types and functions for
///     @ref oac_msg::field::Timestamp field.
struct TimestampCommon
{
    /// @brief Name of the @ref oac_msg::field::Timestamp field.
    static const char* name()
    {
        return "Timestamp";
    }
};

} // namespace field

} // namespace oac_msg
