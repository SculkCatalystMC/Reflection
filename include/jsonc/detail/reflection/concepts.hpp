#pragma once
#include "jsonc/detail/reflection/jsonc_header.hpp"
#include "jsonc/detail/reflection/type_traits.hpp"
#include <variant>

namespace sculk::jsonc::reflection::concepts {

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

template <typename T, bool O, bool A>
concept is_reflectable = traits::is_reflectable_v<std::remove_cvref_t<T>, O, A>;

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
concept is_enum = std::is_enum_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_arithmetic = std::is_arithmetic_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_number = std::is_arithmetic_v<std::remove_cvref_t<T>> && !std::same_as<std::remove_cvref_t<T>, bool>;

template <typename T, bool O, bool A>
concept is_stringifiable_type = traits::is_stringifiable_type_v<std::remove_cvref_t<T>, O, A>;

template <typename T>
concept is_least_stringifiable_type = traits::is_least_stringifiable_type_v<std::remove_cvref_t<T>>;

template <typename F>
concept is_key_formatter = traits::is_key_formatter_v<std::remove_cvref_t<F>>;

template <typename T, typename Arg0>
concept has_call_func_with_arg = requires(T l, Arg0 a) {
    { l.call(a) } noexcept;
};

template <typename T>
concept has_call_func_no_arg = requires(T l) {
    { l.call() } noexcept;
};

template <typename T, typename Arg0>
concept is_dispatcher_listener =
    std::default_initializable<T> && std::is_nothrow_default_constructible_v<T> && (has_call_func_with_arg<T, Arg0> || has_call_func_no_arg<T>);

} // namespace sculk::jsonc::reflection::concepts
