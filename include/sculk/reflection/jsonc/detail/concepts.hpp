#pragma once
#include "sculk/reflection/detail/concepts.hpp"
#include "sculk/reflection/jsonc/detail/type_traits.hpp"
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

namespace sculk::reflection::jsonc {

namespace concepts {

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
concept is_variant = traits::is_variant_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_range_loopable = traits::is_range_loopable_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_aggregate = std::is_aggregate_v<std::remove_cvref_t<T>>;

template <typename T, bool O, bool A>
concept is_stringifiable_type = traits::is_stringifiable_type_v<std::remove_cvref_t<T>, O, A>;

template <typename T>
concept is_least_stringifiable_type = traits::is_least_stringifiable_type_v<std::remove_cvref_t<T>>;

template <typename T, bool O, bool A>
concept is_reflectable = traits::is_reflectable_v<std::remove_cvref_t<T>, O, A>;

template <typename F>
concept is_key_formatter = traits::is_key_formatter_v<std::remove_cvref_t<F>>;

template <typename T>
concept is_jsonc_object = traits::is_jsonc_object_v<T>;

template <typename T>
concept is_jsonc_array = traits::is_jsonc_array_v<T>;

template <typename T>
concept is_jsonc_variant = traits::is_jsonc_variant_v<T>;

template <typename T>
concept is_associative = traits::is_associative_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_string_serializable = traits::is_string_serializable_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_string_convertible = traits::is_string_convertible_v<std::remove_cvref_t<T>>;

} // namespace concepts

namespace detail {

template <concepts::is_least_stringifiable_type T>
constexpr std::string type_to_string(const T& t) noexcept {
    if constexpr (traits::is_string_convertible_v<T>) {
        return t;
    } else if constexpr (traits::is_string_serializable_v<T>) {
        return jsonc::serializer<T>::to_string(t);
    } else {
        if (auto name = magic_enum::enum_name(t); !name.empty()) { return std::string(name); }
        if (auto flag = magic_enum::enum_flags_name(t); !flag.empty()) { return std::string(flag); }
        return std::to_string(std::to_underlying(t));
    }
}

template <concepts::is_least_stringifiable_type T>
constexpr std::optional<T> string_to_type(std::string_view sv) noexcept {
    using RT = std::remove_cvref_t<T>;
    if constexpr (traits::is_string_convertible_v<T>) {
        return std::string(sv);
    } else if constexpr (traits::is_string_serializable_v<T>) {
        return jsonc::serializer<RT>::from_string(sv);
    } else {
        if (auto val = magic_enum::enum_cast<RT>(sv)) { return val; }
        if (auto val = magic_enum::enum_flags_cast<RT>(sv)) { return val; }
        if (auto num = string_utils::str_to_num<std::underlying_type_t<RT>>(sv)) { return static_cast<RT>(*num); }
        return std::nullopt;
    }
}

} // namespace detail

} // namespace sculk::reflection::jsonc