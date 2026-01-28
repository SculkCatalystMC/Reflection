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

namespace {

template <typename T>
constexpr bool has_emplace_back_method_v = requires {
    { std::declval<T>().emplace_back() } -> std::same_as<typename std::remove_cvref_t<T>::value_type&>;
};

template <typename T>
constexpr bool has_insert_method_v = requires(typename std::remove_cvref_t<T>::value_type&& v) { std::declval<T>().insert(v); };

} // namespace

template <typename T>
constexpr bool is_array_like_v = is_range_loopable_v<T> && (has_emplace_back_method_v<T> || has_insert_method_v<T>)
                              && !requires { typename std::remove_cvref_t<T>::mapped_type; } && requires { std::declval<T>().clear(); };

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
constexpr bool is_associative_v = is_range_loopable_v<T> && requires { std::declval<T>().clear(); } && requires {
    typename std::remove_cvref_t<T>::key_type;
    typename std::remove_cvref_t<T>::mapped_type;
};

template <typename T>
constexpr bool is_boolean_serializable_v = requires(const T& t, bool b) {
    { ::jsonc::reflection::Serializer<T>::to_boolean(t) } -> std::convertible_to<bool>;
    requires noexcept(::jsonc::reflection::Serializer<T>::to_boolean(t));
    { ::jsonc::reflection::Serializer<T>::from_boolean(b) } -> std::convertible_to<std::optional<T>>;
    requires noexcept(::jsonc::reflection::Serializer<T>::from_boolean(b));
};

template <typename T>
constexpr bool is_signed_serializable_v = requires(const T& t, int64_t n) {
    { ::jsonc::reflection::Serializer<T>::to_signed(t) } -> std::convertible_to<int64_t>;
    requires noexcept(::jsonc::reflection::Serializer<T>::to_signed(t));
    { ::jsonc::reflection::Serializer<T>::from_signed(n) } -> std::convertible_to<std::optional<T>>;
    requires noexcept(::jsonc::reflection::Serializer<T>::from_signed(n));
};

template <typename T>
constexpr bool is_unsigned_serializable_v = requires(const T& t, uint64_t n) {
    { ::jsonc::reflection::Serializer<T>::to_unsigned(t) } -> std::convertible_to<uint64_t>;
    requires noexcept(::jsonc::reflection::Serializer<T>::to_unsigned(t));
    { ::jsonc::reflection::Serializer<T>::from_unsigned(n) } -> std::convertible_to<std::optional<T>>;
    requires noexcept(::jsonc::reflection::Serializer<T>::from_unsigned(n));
};

template <typename T>
constexpr bool is_float_serializable_v = requires(const T& t, double n) {
    { ::jsonc::reflection::Serializer<T>::to_float(t) } -> std::convertible_to<double>;
    requires noexcept(::jsonc::reflection::Serializer<T>::to_float(t));
    { ::jsonc::reflection::Serializer<T>::from_float(n) } -> std::convertible_to<std::optional<T>>;
    requires noexcept(::jsonc::reflection::Serializer<T>::from_float(n));
};

template <typename T>
constexpr bool is_string_serializable_v = requires(const T& t, std::string_view sv) {
    { ::jsonc::reflection::Serializer<T>::to_string(t) } -> std::convertible_to<std::string>;
    requires noexcept(::jsonc::reflection::Serializer<T>::to_string(t));
    { ::jsonc::reflection::Serializer<T>::from_string(sv) } -> std::convertible_to<std::optional<T>>;
    requires noexcept(::jsonc::reflection::Serializer<T>::from_string(sv));
};

template <typename T>
constexpr bool is_object_serializable_v = requires(const T& t, const Object& o) {
    { ::jsonc::reflection::Serializer<T>::to_object(t) } -> std::convertible_to<Object>;
    requires noexcept(::jsonc::reflection::Serializer<T>::to_object(t));
    { ::jsonc::reflection::Serializer<T>::from_object(o) } -> std::convertible_to<std::optional<T>>;
    requires noexcept(::jsonc::reflection::Serializer<T>::from_object(o));
};

template <typename T>
constexpr bool is_array_serializable_v = requires(const T& t, const Array& a) {
    { ::jsonc::reflection::Serializer<T>::to_array(t) } -> std::convertible_to<Array>;
    requires noexcept(::jsonc::reflection::Serializer<T>::to_array(t));
    { ::jsonc::reflection::Serializer<T>::from_array(a) } -> std::convertible_to<std::optional<T>>;
    requires noexcept(::jsonc::reflection::Serializer<T>::from_array(a));
};

template <typename T>
constexpr bool is_big_int_serializable_v = requires(const T& t, std::string_view n) {
    { ::jsonc::reflection::Serializer<T>::to_big_int(t) } -> std::convertible_to<std::string>;
    requires noexcept(::jsonc::reflection::Serializer<T>::to_big_int(t));
    { ::jsonc::reflection::Serializer<T>::from_big_int(n) } -> std::convertible_to<std::optional<T>>;
    requires noexcept(::jsonc::reflection::Serializer<T>::from_big_int(n));
};

template <typename T>
constexpr bool is_string_convertible_v = std::is_constructible_v<std::string, T> || std::is_convertible_v<std::string, T>;

template <typename T>
constexpr bool is_boolean_type_v =
    is_boolean_serializable_v<T> && !is_signed_serializable_v<T> && !is_unsigned_serializable_v<T> && !is_float_serializable_v<T>
    && !is_string_serializable_v<T> && !is_object_serializable_v<T> && !is_array_serializable_v<T> && !is_big_int_serializable_v<T>;

template <typename T>
constexpr bool is_signed_type_v =
    is_signed_serializable_v<T> && !is_boolean_serializable_v<T> && !is_unsigned_serializable_v<T> && !is_float_serializable_v<T>
    && !is_string_serializable_v<T> && !is_object_serializable_v<T> && !is_array_serializable_v<T> && !is_big_int_serializable_v<T>;

template <typename T>
constexpr bool is_unsigned_type_v =
    is_unsigned_serializable_v<T> && !is_boolean_serializable_v<T> && !is_signed_serializable_v<T> && !is_float_serializable_v<T>
    && !is_string_serializable_v<T> && !is_object_serializable_v<T> && !is_array_serializable_v<T> && !is_big_int_serializable_v<T>;

template <typename T>
constexpr bool is_float_type_v =
    is_float_serializable_v<T> && !is_boolean_serializable_v<T> && !is_signed_serializable_v<T> && !is_unsigned_serializable_v<T>
    && !is_string_serializable_v<T> && !is_object_serializable_v<T> && !is_array_serializable_v<T> && !is_big_int_serializable_v<T>;

template <typename T>
constexpr bool is_string_type_v =
    is_string_serializable_v<T> && !is_boolean_serializable_v<T> && !is_signed_serializable_v<T> && !is_unsigned_serializable_v<T>
    && !is_float_serializable_v<T> && !is_object_serializable_v<T> && !is_array_serializable_v<T> && !is_big_int_serializable_v<T>;

template <typename T>
constexpr bool is_object_type_v =
    is_object_serializable_v<T> && !is_boolean_serializable_v<T> && !is_signed_serializable_v<T> && !is_unsigned_serializable_v<T>
    && !is_float_serializable_v<T> && !is_string_serializable_v<T> && !is_array_serializable_v<T> && !is_big_int_serializable_v<T>;

template <typename T>
constexpr bool is_array_type_v =
    is_array_serializable_v<T> && !is_boolean_serializable_v<T> && !is_signed_serializable_v<T> && !is_unsigned_serializable_v<T>
    && !is_float_serializable_v<T> && !is_string_serializable_v<T> && !is_object_serializable_v<T> && !is_big_int_serializable_v<T>;

template <typename T>
constexpr bool is_big_int_type_v =
    is_big_int_serializable_v<T> && !is_boolean_serializable_v<T> && !is_signed_serializable_v<T> && !is_unsigned_serializable_v<T>
    && !is_float_serializable_v<T> && !is_string_serializable_v<T> && !is_object_serializable_v<T> && !is_array_serializable_v<T>;

template <typename T>
constexpr bool is_jsonc_type_v = std::convertible_to<std::remove_cvref_t<T>, JsoncType>;

template <typename T>
constexpr bool is_stringifiable_type_v = is_string_type_v<T> || is_string_convertible_v<T> || std::is_enum_v<T>;

template <typename T>
constexpr bool is_reflectable_v = std::is_aggregate_v<T> || is_object_serializable_v<T> || is_array_serializable_v<T> || is_associative_v<T>
                               || is_array_like_v<T> || is_tuple_like_v<T>;

template <typename>
struct serializer_arg {};

template <typename R, typename A0>
struct serializer_arg<R (*)(A0) noexcept> {
    using arg_type = A0;
};

template <typename F>
using serializer_arg_t = typename serializer_arg<F>::arg_type;

template <typename F>
constexpr bool is_key_formatter_v = requires(F f, std::string_view sv) {
    { f(sv) } -> std::convertible_to<std::string>;
    requires noexcept(f(sv));
};

} // namespace traits

} // namespace jsonc::reflection