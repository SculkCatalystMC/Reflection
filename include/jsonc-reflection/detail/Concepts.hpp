#pragma once
#include "jsonc-reflection/detail/Jsonc.hpp"
#include "jsonc-reflection/detail/TypeTraits.hpp"
#include <variant>

namespace jsonc_reflection::concepts {

template <typename T>
concept is_renamed = traits::is_renamed_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_ranged = traits::is_ranged_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_optional = traits::is_optional_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_tuple_like = traits::is_tuple_like_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_array_like = traits::is_array_like_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_dispatcher = traits::is_dispatcher_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_reflectable = traits::is_reflectable_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_variant = traits::is_variant_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_range_loopable = traits::is_range_loopable_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_associative = traits::is_associative_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_aggregate = std::is_aggregate_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_jsonc_type_convertible = std::convertible_to<std::remove_cvref_t<T>, jsonc::JsoncType>;

template <typename T>
concept is_string_serializable = traits::is_string_serializable_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_string_convertible = traits::is_string_convertible_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_string_type = traits::is_string_type_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_enum = std::is_enum_v<std::remove_cvref_t<T>>;

} // namespace jsonc_reflection::concepts
