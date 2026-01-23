#pragma once
#include "jsonc-reflection/detail/Ranged.hpp"
#include <array>
#include <concepts>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace jsonc_reflection {

template <size_t N>
struct FixedString;

template <typename T, FixedString AliasName>
class Renamed;

namespace traits {

template <typename...>
constexpr bool always_false = false;

template <typename T>
constexpr bool is_renamed_v = requires { []<typename U, FixedString N>(Renamed<U, N>) {}(std::declval<T>()); };

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
} || requires(T t) {
    begin(t);
    end(t);
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

namespace detail {

template <typename T>
constexpr bool has_from_string_v1 = requires {
    { T::from_string(std::declval<const std::string&>()) } -> std::convertible_to<T>;
};

template <typename T>
constexpr bool has_from_string_v2 = requires {
    { T::fromString(std::declval<const std::string&>()) } -> std::convertible_to<T>;
};

template <typename T>
constexpr bool has_from_string_v3 = requires {
    { T::FromString(std::declval<const std::string&>()) } -> std::convertible_to<T>;
};

template <typename T>
constexpr bool has_to_string_v1 = requires {
    { std::declval<const T>().to_string() } -> std::convertible_to<std::string>;
};

template <typename T>
constexpr bool has_to_string_v2 = requires {
    { std::declval<const T>().toString() } -> std::convertible_to<std::string>;
};

template <typename T>
constexpr bool has_to_string_v3 = requires {
    { std::declval<const T>().ToString() } -> std::convertible_to<std::string>;
};

} // namespace detail

template <typename T>
constexpr bool is_string_serializable_v = (detail::has_from_string_v1<T> || detail::has_from_string_v2<T> || detail::has_from_string_v3<T>)
                                       && (detail::has_to_string_v1<T> || detail::has_to_string_v2<T> || detail::has_to_string_v3<T>);

template <typename T>
constexpr bool is_string_convertible_v = std::is_constructible_v<std::string, T> || std::is_convertible_v<std::string, T>;

template <typename T>
constexpr bool is_string_type_v = is_string_convertible_v<T> || is_string_serializable_v<T>;

} // namespace traits

} // namespace jsonc_reflection