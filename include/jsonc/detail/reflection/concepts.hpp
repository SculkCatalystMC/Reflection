#pragma once
#include "jsonc/detail/reflection/jsonc_header.hpp"
#include "jsonc/detail/reflection/type_traits.hpp"
#include <variant>

namespace jsonc::reflection::concepts {

template <typename T>
concept is_renamed = traits::is_renamed_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_annotated = traits::is_annotated_v<std::remove_cvref_t<T>>;

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
concept is_string_serializable = traits::is_string_serializable_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_string_convertible = traits::is_string_convertible_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_boolean_type = traits::is_boolean_type_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_signed_type = traits::is_signed_type_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_unsigned_type = traits::is_unsigned_type_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_float_type = traits::is_float_type_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_string_type = traits::is_string_type_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_object_type = traits::is_object_type_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_array_type = traits::is_array_type_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_big_int_type = traits::is_big_int_type_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_enum = std::is_enum_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_arithmetic = std::is_arithmetic_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_number = std::is_arithmetic_v<std::remove_cvref_t<T>> && !std::same_as<std::remove_cvref_t<T>, bool>;

template <typename T>
concept is_stringifiable_type = traits::is_stringifiable_type_v<std::remove_cvref_t<T>>;

} // namespace jsonc::reflection::concepts
