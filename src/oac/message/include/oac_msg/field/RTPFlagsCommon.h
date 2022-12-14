// Generated by commsdsl2comms v5.0.1

/// @file
/// @brief Contains common template parameters independent functionality of
///    @ref oac_msg::field::RTPFlags field.

#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

namespace oac_msg
{

namespace field
{

/// @brief Scope for all the common definitions of the member fields of
///     @ref oac_msg::field::RTPFlags field.
struct RTPFlagsMembersCommon
{
    /// @brief Common types and functions for
    ///     @ref oac_msg::field::RTPFlagsMembers::Version field.
    struct VersionCommon
    {
        /// @brief Re-definition of the value type used by
        ///     oac_msg::field::RTPFlagsMembers::Version field.
        using ValueType = std::uint8_t;

        /// @brief Name of the @ref oac_msg::field::RTPFlagsMembers::Version field.
        static const char* name()
        {
            return "version";
        }

        /// @brief Compile time detection of special values presence.
        static constexpr bool hasSpecials()
        {
            return false;
        }
    };

    /// @brief Common types and functions for
    ///     @ref oac_msg::field::RTPFlagsMembers::Padding field.
    struct PaddingCommon
    {
        /// @brief Re-definition of the value type used by
        ///     oac_msg::field::RTPFlagsMembers::Padding field.
        using ValueType = std::uint8_t;

        /// @brief Name of the @ref oac_msg::field::RTPFlagsMembers::Padding field.
        static const char* name()
        {
            return "padding";
        }

        /// @brief Compile time detection of special values presence.
        static constexpr bool hasSpecials()
        {
            return false;
        }
    };

    /// @brief Common types and functions for
    ///     @ref oac_msg::field::RTPFlagsMembers::Extension field.
    struct ExtensionCommon
    {
        /// @brief Re-definition of the value type used by
        ///     oac_msg::field::RTPFlagsMembers::Extension field.
        using ValueType = std::uint8_t;

        /// @brief Name of the @ref oac_msg::field::RTPFlagsMembers::Extension field.
        static const char* name()
        {
            return "extension";
        }

        /// @brief Compile time detection of special values presence.
        static constexpr bool hasSpecials()
        {
            return false;
        }
    };

    /// @brief Common types and functions for
    ///     @ref oac_msg::field::RTPFlagsMembers::Csrc_count field.
    struct Csrc_countCommon
    {
        /// @brief Re-definition of the value type used by
        ///     oac_msg::field::RTPFlagsMembers::Csrc_count field.
        using ValueType = std::uint8_t;

        /// @brief Name of the @ref oac_msg::field::RTPFlagsMembers::Csrc_count field.
        static const char* name()
        {
            return "csrc_count";
        }

        /// @brief Compile time detection of special values presence.
        static constexpr bool hasSpecials()
        {
            return false;
        }
    };

    /// @brief Common types and functions for
    ///     @ref oac_msg::field::RTPFlagsMembers::Marker field.
    struct MarkerCommon
    {
        /// @brief Re-definition of the value type used by
        ///     oac_msg::field::RTPFlagsMembers::Marker field.
        using ValueType = std::uint8_t;

        /// @brief Name of the @ref oac_msg::field::RTPFlagsMembers::Marker field.
        static const char* name()
        {
            return "marker";
        }

        /// @brief Compile time detection of special values presence.
        static constexpr bool hasSpecials()
        {
            return false;
        }
    };

    /// @brief Common types and functions for
    ///     @ref oac_msg::field::RTPFlagsMembers::Payload_type field.
    struct Payload_typeCommon
    {
        /// @brief Values enumerator for
        ///     @ref oac_msg::field::RTPFlagsMembers::Payload_type field.
        enum class ValueType : std::uint8_t
        {
            PCMU = 0, ///< ITU-T G.711 PCM ??-Law audio 64
                     ///  kbit/s 
            GSM = 3, ///< European GSM Full Rate audio 13
                    ///  kbit/s (GSM 06.10) 
            G723 = 4, ///< ITU-T G.723.1 audio 
            DVI4_32 = 5, ///< IMA ADPCM audio 32 kbit/s 
            DVI4_64 = 6, ///< IMA ADPCM audio 64 kbit/s 
            LPC = 7, ///< Experimental Linear Predictive Coding
                    ///  audio 5.6 kbit/s 
            PCMA = 8, ///< ITU-T G.711 PCM A-Law audio 64
                     ///  kbit/s 
            G722 = 9, ///< ITU-T G.722 audio 64 kbit/s 
            L16_stereo = 10, ///< Linear PCM 16-bit Stereo audio 1411.2
                            ///  kbit/s, uncompressed 
            L16_mono = 11, ///< Linear PCM 16-bit audio 705.6 kbit/s,
                          ///  uncompressed 
            QCELP = 12, ///< Qualcomm Code Excited Linear
                       ///  Prediction 
            CN = 13, ///< Comfort noise. Payload type used
                    ///  with audio codecs that do not support
                    ///  comfort noise as part of the codec itself
                    ///  such as G.711, G.722.1, G.722, G.726,
                    ///  G.727, G.728, GSM 06.10, Siren, and
                    ///  RTAudio. 
            MPA = 14, ///< MPEG-1 or MPEG-2 audio only 
            G728 = 15, ///< ITU-T G.728 audio 16 kbit/s 
            DVI4_44 = 16, ///< IMA ADPCM audio 44.1 kbit/s 
            DVI4_88 = 17, ///< IMA ADPCM audio 88.2 kbit/s 
            G729 = 18, ///< ITU-T G.729 and G.729a audio 8
                      ///  kbit/s; Annex B is implied unless the
                      ///  annexb=no parameter is used 
            CELLB = 25, ///< Sun CellB video 
            JPEG = 26, ///< JPEG video 
            nv = 28, ///< Xerox PARC's Network Video 
            H261 = 31, ///< ITU-T H.261 video 
            MPV = 32, ///< MPEG-1 and MPEG-2 video 
            MP2T = 33, ///< MPEG-2 transport stream 
            H263 = 34, ///< H.263 video, first version (1996) 

            // --- Extra values generated for convenience ---
            FirstValue = 0, ///< First defined value.
            LastValue = 34, ///< Last defined value.
            ValuesLimit = 35, ///< Upper limit for defined values.
        };

        /// @brief Single value name info entry
        using ValueNameInfo = std::pair<ValueType, const char*>;

        /// @brief Type returned from @ref valueNamesMap() member function.
        /// @details The @b first value of the pair is pointer to the map array,
        ///     The @b second value of the pair is the size of the array.
        using ValueNamesMapInfo = std::pair<const ValueNameInfo*, std::size_t>;

        /// @brief Name of the @ref oac_msg::field::RTPFlagsMembers::Payload_type field.
        static const char* name()
        {
            return "payload_type";
        }

        /// @brief Retrieve name of the enum value
        static const char* valueName(ValueType val)
        {
            auto namesMapInfo = valueNamesMap();
            auto begIter = namesMapInfo.first;
            auto endIter = begIter + namesMapInfo.second;
            auto iter = std::lower_bound(
                begIter, endIter, val,
                [](const ValueNameInfo& info, ValueType v) -> bool
                {
                    return info.first < v;
                });

            if ((iter == endIter) || (iter->first != val)) {
                return nullptr;
            }

            return iter->second;
        }

        /// @brief Retrieve map of enum value names
        static ValueNamesMapInfo valueNamesMap()
        {
            static const ValueNameInfo Map[] = {
                std::make_pair(ValueType::PCMU, "PCMU"),
                std::make_pair(ValueType::GSM, "GSM"),
                std::make_pair(ValueType::G723, "G723"),
                std::make_pair(ValueType::DVI4_32, "DVI4_32"),
                std::make_pair(ValueType::DVI4_64, "DVI4_64"),
                std::make_pair(ValueType::LPC, "LPC"),
                std::make_pair(ValueType::PCMA, "PCMA"),
                std::make_pair(ValueType::G722, "G722"),
                std::make_pair(ValueType::L16_stereo, "L16_stereo"),
                std::make_pair(ValueType::L16_mono, "L16_mono"),
                std::make_pair(ValueType::QCELP, "QCELP"),
                std::make_pair(ValueType::CN, "CN"),
                std::make_pair(ValueType::MPA, "MPA"),
                std::make_pair(ValueType::G728, "G728"),
                std::make_pair(ValueType::DVI4_44, "DVI4_44"),
                std::make_pair(ValueType::DVI4_88, "DVI4_88"),
                std::make_pair(ValueType::G729, "G729"),
                std::make_pair(ValueType::CELLB, "CELLB"),
                std::make_pair(ValueType::JPEG, "JPEG"),
                std::make_pair(ValueType::nv, "nv"),
                std::make_pair(ValueType::H261, "H261"),
                std::make_pair(ValueType::MPV, "MPV"),
                std::make_pair(ValueType::MP2T, "MP2T"),
                std::make_pair(ValueType::H263, "H263")
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
        }
    };

    /// @brief Values enumerator for
    ///     @ref oac_msg::field::RTPFlagsMembers::Payload_type field.
    using Payload_typeVal = Payload_typeCommon::ValueType;
};

/// @brief Common types and functions for
///     @ref oac_msg::field::RTPFlags field.
struct RTPFlagsCommon
{
    /// @brief Name of the @ref oac_msg::field::RTPFlags field.
    static const char* name()
    {
        return "RTPFlags";
    }
};

} // namespace field

} // namespace oac_msg
