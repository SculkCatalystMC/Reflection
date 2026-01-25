#pragma once
#include "jsonc/detail/reflection/ranged.hpp"
#include "jsonc/detail/reflection/serializer.hpp"
#include <array>
#include <concepts>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace jsonc::reflection {

template <size_t N>
struct FixedString;

template <typename T, FixedString AliasName>
class Renamed;

namespace traits {

template <typename T>
constexpr bool always_false_v = false;

template <typename T>
constexpr bool is_renamed_v = requires {
    { std::declval<const T>().view() } -> std::convertible_to<std::string_view>;
    typename std::remove_cvref_t<T>::value_type;
};

template <typename T>
constexpr bool is_annotated_v = requires {
    { std::declval<const T>().get_comments() } -> std::convertible_to<std::vector<std::string>>;
    typename std::remove_cvref_t<T>::value_type;
};

template <typename>
struct is_ranged_t : std::false_type {};

template <typename T, FixedNumber<T> Min, FixedNumber<T> Max>
struct is_ranged_t<Ranged<T, Min, Max>> : std::true_type {};

template <typename T>
constexpr bool is_ranged_v = is_ranged_t<T>::value;

template <typename T>
constexpr bool is_optional_v = requires(T o) {
    o.value();
    o.has_value();
    o.operator*();
    typename std::remove_cvref_t<T>::value_type;
};

template <typename T>
constexpr bool is_range_loopable_v = std::is_bounded_array_v<std::remove_cvref_t<T>> || requires(T t) {
    t.begin();
    t.end();
};

template <typename T>
constexpr bool is_tuple_like_v = requires(T t) {
    std::tuple_size<std::remove_cvref_t<T>>::value;
    []<std::size_t... I>(T&& t, std::index_sequence<I...>) {
        ((void)std::get<I>(std::forward<T>(t)), ...);
    }(std::forward<T>(t), std::make_index_sequence<std::tuple_size<std::remove_cvref_t<T>>::value>{});
};

template <typename T>
constexpr bool is_array_like_v = is_range_loopable_v<T> && !requires { typename std::remove_cvref_t<T>::mapped_type; };

template <typename T>
constexpr bool is_dispatcher_v = requires(T t) {
    typename std::remove_cvref_t<T>::storage_type;
    typename std::remove_cvref_t<T>::listener_type;
    t.storage_;
    t.call();
};

template <typename T>
constexpr bool is_variant_v = requires { typename std::variant_size<std::remove_cvref_t<T>>::type; };

template <typename T>
constexpr bool is_reflectable_v = std::is_aggregate_v<std::remove_cvref_t<T>>;

template <typename T>
constexpr bool is_associative_v = is_range_loopable_v<T> && requires {
    typename std::remove_cvref_t<T>::key_type;
    typename std::remove_cvref_t<T>::mapped_type;
};

template <typename T>
constexpr bool is_string_serializable_v = requires(const T& t, std::string_view sv) {
    { ::jsonc::reflection::Serializer<T>::to_string(t) } -> std::convertible_to<std::string>;
    requires noexcept(::jsonc::reflection::Serializer<T>::to_string(t));
    { ::jsonc::reflection::Serializer<T>::from_string(sv) } -> std::convertible_to<std::optional<T>>;
    requires noexcept(::jsonc::reflection::Serializer<T>::from_string(sv));
};

template <typename T>
constexpr bool is_string_convertible_v = std::is_constructible_v<std::string, T> || std::is_convertible_v<std::string, T>;

template <typename T>
constexpr bool is_string_type_v = is_string_convertible_v<T> || is_string_serializable_v<T> || std::is_enum_v<T>;

template <typename T>
constexpr bool is_jsonc_type_v = std::convertible_to<std::remove_cvref_t<T>, JsoncType>;

} // namespace traits

} // namespace jsonc::reflection