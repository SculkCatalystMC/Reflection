#pragma once
#include "jsonc-reflection/detail/Ranged.hpp"
#include <array>
#include <concepts>
#include <string>
#include <type_traits>
#include <utility>

namespace jsonc_reflection::traits {

template <typename T, template <class...> typename Z, typename... Ts>
concept Require = Z<T, Ts...>::value;

template <typename>
constexpr bool is_std_array_v = false;

template <typename T, size_t N>
constexpr bool is_std_array_v<::std::array<T, N>> = true;

template <typename T>
constexpr bool is_reflectable_v = std::is_aggregate_v<std::remove_cvref_t<T>> && !is_std_array_v<std::remove_cvref_t<T>>;

template <typename>
struct is_ranged : std::false_type {};

template <typename T, FixedNumber<T> Min, FixedNumber<T> Max>
struct is_ranged<Ranged<T, Min, Max>> : std::true_type {};

template <typename T>
constexpr bool is_string_v = std::is_constructible_v<std::string, T>;

template <typename T, typename U>
struct is_in_types;

template <typename T, template <typename...> typename U, typename... Ts>
struct is_in_types<T, U<Ts...>> : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

template <typename T, typename U>
constexpr bool is_in_types_v = is_in_types<T, U>::value;

template <typename T, typename... Ts>
constexpr bool is_one_of_v = (std::is_same_v<T, Ts> || ...);

template <typename T, typename... Ts>
struct is_one_of : std::bool_constant<is_one_of_v<T, Ts...>> {};

template <typename T, typename... Ts>
constexpr bool is_all_same_v = (std::is_same_v<T, Ts> && ...);

template <typename T, typename... Ts>
struct is_all_same : std::bool_constant<is_all_same_v<T, Ts...>> {};

template <typename T, template <typename...> typename Z>
constexpr bool is_specialization_of_v = false;

template <template <typename...> typename Z, typename... Args>
constexpr bool is_specialization_of_v<Z<Args...>, Z> = true;

template <typename T, template <typename...> typename Z>
struct is_specialization_of : std::bool_constant<is_specialization_of_v<T, Z>> {};

template <typename...>
constexpr bool always_false = false;

template <class T>
constexpr bool is_awaiter_v = requires(std::remove_cv_t<T>& t) {
    { t.await_ready() } -> std::same_as<bool>;
    t.await_resume();
};
template <class T>
struct is_awaiter : std::bool_constant<is_awaiter_v<T>> {};

template <class T>
constexpr bool has_member_operator_co_await_v = requires(T t) {
    { t.operator co_await() } -> Require<is_awaiter>;
};
template <class T>
struct has_member_operator_co_await : std::bool_constant<has_member_operator_co_await_v<T>> {};

template <class T>
constexpr bool has_non_member_operator_co_await_v = requires(T t) {
    { operator co_await(t) } -> Require<is_awaiter>;
};
template <class T>
struct has_non_member_operator_co_await : std::bool_constant<has_non_member_operator_co_await_v<T>> {};

template <class T>
constexpr bool is_awaitable_v = has_member_operator_co_await_v<T> || has_non_member_operator_co_await_v<T> || is_awaiter_v<T>;

template <class T>
struct is_awaitable : std::bool_constant<is_awaitable_v<T>> {};

} // namespace jsonc_reflection::traits