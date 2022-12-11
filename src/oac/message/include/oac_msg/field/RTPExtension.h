// Generated by commsdsl2comms v5.0.1

/// @file
/// @brief Contains definition of <b>"RTPExtension"</b> field.

#pragma once

#include <algorithm>
#include <cstdint>
#include <limits>
#include <tuple>
#include "comms/Assert.h"
#include "comms/CompileControl.h"
#include "comms/field/ArrayList.h"
#include "comms/field/Bundle.h"
#include "comms/field/IntValue.h"
#include "comms/field/Variant.h"
#include "comms/options.h"
#include "oac_msg/field/FieldBase.h"
#include "oac_msg/field/RTPExtensionCommon.h"
#include "oac_msg/field/Timestamp.h"
#include "oac_msg/options/DefaultOptions.h"

namespace oac_msg
{

namespace field
{

/// @brief Scope for all the member fields of
///     @ref RTPExtension field.
/// @tparam TOpt Protocol options.
template <typename TOpt = oac_msg::options::DefaultOptions>
struct RTPExtensionMembers
{
    /// @brief Scope for all the member fields of
    ///     @ref Generic field.
    struct GenericMembers
    {
        /// @brief Definition of <b>"extension_id"</b> field.
        class Extension_id : public
            comms::field::IntValue<
                oac_msg::field::FieldBase<>,
                std::uint16_t,
                comms::option::def::FailOnInvalid<>,
                comms::option::def::ValidNumValue<0>,
                comms::option::def::HasCustomRefresh
            >
        {
            using Base =
                comms::field::IntValue<
                    oac_msg::field::FieldBase<>,
                    std::uint16_t,
                    comms::option::def::FailOnInvalid<>,
                    comms::option::def::ValidNumValue<0>,
                    comms::option::def::HasCustomRefresh
                >;
        public:
            /// @brief Re-definition of the value type.
            using ValueType = typename Base::ValueType;

            /// @brief Compile time detection of special values presence.
            static constexpr bool hasSpecials()
            {
                return oac_msg::field::RTPExtensionMembersCommon::GenericMembersCommon::Extension_idCommon::hasSpecials();
            }

            /// @brief Name of the field.
            static const char* name()
            {
                return oac_msg::field::RTPExtensionMembersCommon::GenericMembersCommon::Extension_idCommon::name();
            }

            /// @brief Generated refresh functionality.
            bool refresh()
            {
                bool updated = Base::refresh();
                if (Base::valid()) {
                    return updated;
                };
                Base::setValue(0);
                return true;
            }
        };

        /// @brief Definition of <b>"length"</b> field.
        class Length : public
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
                return oac_msg::field::RTPExtensionMembersCommon::GenericMembersCommon::LengthCommon::hasSpecials();
            }

            /// @brief Name of the field.
            static const char* name()
            {
                return oac_msg::field::RTPExtensionMembersCommon::GenericMembersCommon::LengthCommon::name();
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
                    return oac_msg::field::RTPExtensionMembersCommon::GenericMembersCommon::ContentMembersCommon::ElementCommon::hasSpecials();
                }

                /// @brief Name of the field.
                static const char* name()
                {
                    return oac_msg::field::RTPExtensionMembersCommon::GenericMembersCommon::ContentMembersCommon::ElementCommon::name();
                }
            };
        };

        /// @brief Definition of <b>"content"</b> field.
        class Content : public
            comms::field::ArrayList<
                oac_msg::field::FieldBase<>,
                typename ContentMembers::Element,
                typename TOpt::field::RTPExtensionMembers::GenericMembers::Content,
                comms::option::def::SequenceSizeForcingEnabled
            >
        {
            using Base =
                comms::field::ArrayList<
                    oac_msg::field::FieldBase<>,
                    typename ContentMembers::Element,
                    typename TOpt::field::RTPExtensionMembers::GenericMembers::Content,
                    comms::option::def::SequenceSizeForcingEnabled
                >;
        public:
            /// @brief Name of the field.
            static const char* name()
            {
                return oac_msg::field::RTPExtensionMembersCommon::GenericMembersCommon::ContentCommon::name();
            }
        };

        /// @brief All members bundled in @b std::tuple.
        using All =
            std::tuple<
               Extension_id,
               Length,
               Content
            >;
    };

    /// @brief Definition of <b>"generic"</b> field.
    class Generic : public
        comms::field::Bundle<
            oac_msg::field::FieldBase<>,
            typename GenericMembers::All,
            comms::option::def::HasCustomRead,
            comms::option::def::HasCustomRefresh
        >
    {
        using Base =
            comms::field::Bundle<
                oac_msg::field::FieldBase<>,
                typename GenericMembers::All,
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
        ///     @li @b FieldIdx_extension_id index, @b Field_extension_id type and @b field_extension_id() access function -
        ///         for oac_msg::field::RTPExtensionMembers::GenericMembers::Extension_id member field.
        ///     @li @b FieldIdx_length index, @b Field_length type and @b field_length() access function -
        ///         for oac_msg::field::RTPExtensionMembers::GenericMembers::Length member field.
        ///     @li @b FieldIdx_content index, @b Field_content type and @b field_content() access function -
        ///         for oac_msg::field::RTPExtensionMembers::GenericMembers::Content member field.
        COMMS_FIELD_MEMBERS_NAMES(
            extension_id,
            length,
            content
        );

        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::field::RTPExtensionMembersCommon::GenericCommon::name();
        }

        /// @brief Generated read functionality.
        template <typename TIter>
        comms::ErrorStatus read(TIter& iter, std::size_t len)
        {
            auto es = comms::ErrorStatus::Success;
            do {
                es = Base::template readUntilAndUpdateLen<FieldIdx_content>(iter, len);
                if (es != comms::ErrorStatus::Success) {
                    break;
                }

                readPrepare_content();

                es = Base::template readFrom<FieldIdx_content>(iter, len);
            } while(false);
            return es;
        }

        /// @brief Generated refresh functionality.
        bool refresh()
        {
            bool updated = Base::refresh();
            updated = refresh_content() || updated;
            return updated;
        }

    private:
        void readPrepare_content()
        {
            field_content().forceReadElemCount(
                static_cast<std::size_t>(field_length().getValue()));
        }

        bool refresh_content()
        {
            bool updated = false;
            do {
                auto expectedValue = static_cast<std::size_t>(field_length().getValue());
                auto realValue = field_content().value().size();
                if (expectedValue == realValue) {
                    break;
                }

                using PrefixValueType = typename std::decay<decltype(field_length().getValue())>::type;
                static const auto MaxPrefixValue = static_cast<std::size_t>(std::numeric_limits<PrefixValueType>::max());
                auto maxAllowedValue = std::min(MaxPrefixValue, realValue);
                if (maxAllowedValue < realValue) {
                    field_content().value().resize(maxAllowedValue);
                }
                field_length().setValue(maxAllowedValue);
                updated = true;
            } while (false);

            return updated;
        }
    };

    /// @brief Scope for all the member fields of
    ///     @ref Oac_extension field.
    struct Oac_extensionMembers
    {
        /// @brief Definition of <b>"extension_id"</b> field.
        class Extension_id : public
            comms::field::IntValue<
                oac_msg::field::FieldBase<>,
                std::uint16_t,
                comms::option::def::FailOnInvalid<>,
                comms::option::def::DefaultNumValue<4919>,
                comms::option::def::ValidNumValue<4919>,
                comms::option::def::HasCustomRefresh
            >
        {
            using Base =
                comms::field::IntValue<
                    oac_msg::field::FieldBase<>,
                    std::uint16_t,
                    comms::option::def::FailOnInvalid<>,
                    comms::option::def::DefaultNumValue<4919>,
                    comms::option::def::ValidNumValue<4919>,
                    comms::option::def::HasCustomRefresh
                >;
        public:
            /// @brief Re-definition of the value type.
            using ValueType = typename Base::ValueType;

            /// @brief Compile time detection of special values presence.
            static constexpr bool hasSpecials()
            {
                return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionMembersCommon::Extension_idCommon::hasSpecials();
            }

            /// @brief Name of the field.
            static const char* name()
            {
                return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionMembersCommon::Extension_idCommon::name();
            }

            /// @brief Generated refresh functionality.
            bool refresh()
            {
                bool updated = Base::refresh();
                if (Base::valid()) {
                    return updated;
                };
                Base::setValue(4919);
                return true;
            }
        };

        /// @brief Definition of <b>"length"</b> field.
        class Length : public
            comms::field::IntValue<
                oac_msg::field::FieldBase<>,
                std::uint16_t,
                comms::option::def::DefaultNumValue<4>,
                comms::option::def::ValidNumValue<4>
            >
        {
            using Base =
                comms::field::IntValue<
                    oac_msg::field::FieldBase<>,
                    std::uint16_t,
                    comms::option::def::DefaultNumValue<4>,
                    comms::option::def::ValidNumValue<4>
                >;
        public:
            /// @brief Re-definition of the value type.
            using ValueType = typename Base::ValueType;

            /// @brief Compile time detection of special values presence.
            static constexpr bool hasSpecials()
            {
                return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionMembersCommon::LengthCommon::hasSpecials();
            }

            /// @brief Name of the field.
            static const char* name()
            {
                return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionMembersCommon::LengthCommon::name();
            }
        };

        /// @brief Definition of <b>"version"</b> field.
        class Version : public
            comms::field::IntValue<
                oac_msg::field::FieldBase<>,
                std::uint16_t,
                comms::option::def::DefaultNumValue<1>
            >
        {
            using Base =
                comms::field::IntValue<
                    oac_msg::field::FieldBase<>,
                    std::uint16_t,
                    comms::option::def::DefaultNumValue<1>
                >;
        public:
            /// @brief Re-definition of the value type.
            using ValueType = typename Base::ValueType;

            /// @brief Compile time detection of special values presence.
            static constexpr bool hasSpecials()
            {
                return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionMembersCommon::VersionCommon::hasSpecials();
            }

            /// @brief Name of the field.
            static const char* name()
            {
                return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionMembersCommon::VersionCommon::name();
            }
        };

        /// @brief Definition of <b>"dntp_server_port"</b> field.
        class Dntp_server_port : public
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
                return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionMembersCommon::Dntp_server_portCommon::hasSpecials();
            }

            /// @brief Name of the field.
            static const char* name()
            {
                return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionMembersCommon::Dntp_server_portCommon::name();
            }
        };

        /// @brief Scope for all the member fields of
        ///     @ref Dntp_server_ipv4_address field.
        struct Dntp_server_ipv4_addressMembers
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
                    return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionMembersCommon::Dntp_server_ipv4_addressMembersCommon::ElementCommon::hasSpecials();
                }

                /// @brief Name of the field.
                static const char* name()
                {
                    return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionMembersCommon::Dntp_server_ipv4_addressMembersCommon::ElementCommon::name();
                }
            };
        };

        /// @brief Definition of <b>"dntp_server_ipv4_address"</b> field.
        class Dntp_server_ipv4_address : public
            comms::field::ArrayList<
                oac_msg::field::FieldBase<>,
                typename Dntp_server_ipv4_addressMembers::Element,
                typename TOpt::field::RTPExtensionMembers::Oac_extensionMembers::Dntp_server_ipv4_address,
                comms::option::def::SequenceFixedSize<4U>
            >
        {
            using Base =
                comms::field::ArrayList<
                    oac_msg::field::FieldBase<>,
                    typename Dntp_server_ipv4_addressMembers::Element,
                    typename TOpt::field::RTPExtensionMembers::Oac_extensionMembers::Dntp_server_ipv4_address,
                    comms::option::def::SequenceFixedSize<4U>
                >;
        public:
            /// @brief Name of the field.
            static const char* name()
            {
                return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionMembersCommon::Dntp_server_ipv4_addressCommon::name();
            }
        };

        /// @brief Definition of <b>"reference_timestamp"</b> field.
        class Reference_timestamp : public
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
                return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionMembersCommon::Reference_timestampCommon::name();
            }
        };

        /// @brief All members bundled in @b std::tuple.
        using All =
            std::tuple<
               Extension_id,
               Length,
               Version,
               Dntp_server_port,
               Dntp_server_ipv4_address,
               Reference_timestamp
            >;
    };

    /// @brief Definition of <b>"oac_extension"</b> field.
    class Oac_extension : public
        comms::field::Bundle<
            oac_msg::field::FieldBase<>,
            typename Oac_extensionMembers::All
        >
    {
        using Base =
            comms::field::Bundle<
                oac_msg::field::FieldBase<>,
                typename Oac_extensionMembers::All
            >;
    public:
        /// @brief Allow access to internal fields.
        /// @details See definition of @b COMMS_FIELD_MEMBERS_NAMES macro
        ///     related to @b comms::field::Bundle class from COMMS library
        ///     for details.
        ///
        ///     The generated values, types and access functions are:
        ///     @li @b FieldIdx_extension_id index, @b Field_extension_id type and @b field_extension_id() access function -
        ///         for oac_msg::field::RTPExtensionMembers::Oac_extensionMembers::Extension_id member field.
        ///     @li @b FieldIdx_length index, @b Field_length type and @b field_length() access function -
        ///         for oac_msg::field::RTPExtensionMembers::Oac_extensionMembers::Length member field.
        ///     @li @b FieldIdx_version index, @b Field_version type and @b field_version() access function -
        ///         for oac_msg::field::RTPExtensionMembers::Oac_extensionMembers::Version member field.
        ///     @li @b FieldIdx_dntp_server_port index, @b Field_dntp_server_port type and @b field_dntp_server_port() access function -
        ///         for oac_msg::field::RTPExtensionMembers::Oac_extensionMembers::Dntp_server_port member field.
        ///     @li @b FieldIdx_dntp_server_ipv4_address index, @b Field_dntp_server_ipv4_address type and @b field_dntp_server_ipv4_address() access function -
        ///         for oac_msg::field::RTPExtensionMembers::Oac_extensionMembers::Dntp_server_ipv4_address member field.
        ///     @li @b FieldIdx_reference_timestamp index, @b Field_reference_timestamp type and @b field_reference_timestamp() access function -
        ///         for oac_msg::field::RTPExtensionMembers::Oac_extensionMembers::Reference_timestamp member field.
        COMMS_FIELD_MEMBERS_NAMES(
            extension_id,
            length,
            version,
            dntp_server_port,
            dntp_server_ipv4_address,
            reference_timestamp
        );

        /// @brief Name of the field.
        static const char* name()
        {
            return oac_msg::field::RTPExtensionMembersCommon::Oac_extensionCommon::name();
        }
    };

    /// @brief All members bundled in @b std::tuple.
    using All =
        std::tuple<
           Generic,
           Oac_extension
        >;
};

/// @brief Definition of <b>"RTPExtension"</b> field.
/// @tparam TOpt Protocol options.
/// @tparam TExtraOpts Extra options.
template <typename TOpt = oac_msg::options::DefaultOptions, typename... TExtraOpts>
class RTPExtension : public
    comms::field::Variant<
        oac_msg::field::FieldBase<>,
        typename RTPExtensionMembers<TOpt>::All,
        TExtraOpts...,
        comms::option::def::HasCustomRead
    >
{
    using Base =
        comms::field::Variant<
            oac_msg::field::FieldBase<>,
            typename RTPExtensionMembers<TOpt>::All,
            TExtraOpts...,
            comms::option::def::HasCustomRead
        >;
public:
    /// @brief Allow access to internal fields.
    /// @details See definition of @b COMMS_VARIANT_MEMBERS_NAMES macro
    ///     related to @b comms::field::Variant class from COMMS library
    ///     for details.
    ///
    ///     The generated values, types and access functions are:
    ///     @li @b FieldIdx_generic index, @b Field_generic type,
    ///         @b initField_generic() and @b accessField_generic() access functions -
    ///         for oac_msg::field::RTPExtensionMembers::Generic member field.
    ///     @li @b FieldIdx_oac_extension index, @b Field_oac_extension type,
    ///         @b initField_oac_extension() and @b accessField_oac_extension() access functions -
    ///         for oac_msg::field::RTPExtensionMembers::Oac_extension member field.
    COMMS_VARIANT_MEMBERS_NAMES(
        generic,
        oac_extension
    );

    /// @brief Optimized currFieldExec functionality.
    /// @details Replaces the currFieldExec() member function defined
    ///    by @b comms::field::Variant.
    template <typename TFunc>
    void currFieldExec(TFunc&& func) 
    {
        switch (Base::currentField()) {
        case FieldIdx_generic:
            memFieldDispatch<FieldIdx_generic>(accessField_generic(), std::forward<TFunc>(func));
            break;
        case FieldIdx_oac_extension:
            memFieldDispatch<FieldIdx_oac_extension>(accessField_oac_extension(), std::forward<TFunc>(func));
            break;
        default:
            static constexpr bool Invalid_field_execution = false;
            static_cast<void>(Invalid_field_execution);
            COMMS_ASSERT(Invalid_field_execution);
            break;
        }
    }

    /// @brief Optimized currFieldExec functionality (const variant).
    /// @details Replaces the currFieldExec() member function defined
    ///    by @b comms::field::Variant.
    template <typename TFunc>
    void currFieldExec(TFunc&& func) const
    {
        switch (Base::currentField()) {
        case FieldIdx_generic:
            memFieldDispatch<FieldIdx_generic>(accessField_generic(), std::forward<TFunc>(func));
            break;
        case FieldIdx_oac_extension:
            memFieldDispatch<FieldIdx_oac_extension>(accessField_oac_extension(), std::forward<TFunc>(func));
            break;
        default:
            static constexpr bool Invalid_field_execution = false;
            static_cast<void>(Invalid_field_execution);
            COMMS_ASSERT(Invalid_field_execution);
            break;
        }
    }

    /// @brief Name of the field.
    static const char* name()
    {
        return oac_msg::field::RTPExtensionCommon::name();
    }

    COMMS_MSVC_WARNING_PUSH
    COMMS_MSVC_WARNING_DISABLE(4702)
    /// @brief Generated read functionality.
    template <typename TIter>
    comms::ErrorStatus read(TIter& iter, std::size_t len)
    {
        using CommonKeyField=
            comms::field::IntValue<
                oac_msg::field::FieldBase<>,
                std::uint16_t,
                comms::option::def::FailOnInvalid<>
            >;
        CommonKeyField commonKeyField;

        auto origIter = iter;
        auto es = commonKeyField.read(iter, len);
        if (es != comms::ErrorStatus::Success) {
            return es;
        }

        auto consumedLen = static_cast<std::size_t>(std::distance(origIter, iter));
        COMMS_ASSERT(consumedLen <= len);
        len -= consumedLen;

        switch (commonKeyField.getValue()) {
        case 0U /* 0x0000U */:
            {
                auto& field_generic = initField_generic();
                COMMS_ASSERT(field_generic.field_extension_id().getValue() == commonKeyField.getValue());
                return field_generic.template readFrom<1>(iter, len);
            }
        case 4919U /* 0x1337U */:
            {
                auto& field_oac_extension = initField_oac_extension();
                COMMS_ASSERT(field_oac_extension.field_extension_id().getValue() == commonKeyField.getValue());
                return field_oac_extension.template readFrom<1>(iter, len);
            }
        default:
            break;
        };

        return comms::ErrorStatus::InvalidMsgData;
    }
    COMMS_MSVC_WARNING_POP

private:
    template <std::size_t TIdx, typename TField, typename TFunc>
    static void memFieldDispatch(TField&& f, TFunc&& func)
    {
        #ifdef _MSC_VER
            func.operator()<TIdx>(std::forward<TField>(f)); // VS compiler
        #else // #ifdef _MSC_VER
            func.template operator()<TIdx>(std::forward<TField>(f)); // All other compilers
        #endif // #ifdef _MSC_VER
    }
};

} // namespace field

} // namespace oac_msg
