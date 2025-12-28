#pragma once
#include "TypeTraits.hpp"
#include <variant>

namespace jsonc_reflection {

template <size_t N>
struct FixedString;

template <typename T, FixedString... Comments>
class Annotated;

template <typename T, FixedString AliasName>
class Renamed;

namespace concepts {

template <typename T>
concept IsAnnotated = requires { []<typename U, FixedString... Comments>(Annotated<U, Comments...>) {}(std::declval<T>()); };

template <typename T>
concept IsRenamed = requires { []<typename U, FixedString AliasName>(Renamed<U, AliasName>) {}(std::declval<T>()); };

template <typename T>
concept IsRanged = traits::is_ranged<T>::value;

template <typename T>
concept IsReflectable = traits::is_reflectable_v<T>;

template <typename T, typename U>
concept IsInTypes = traits::is_in_types_v<T, U>;

template <typename T, typename... Ts>
concept IsOneOf = traits::is_one_of_v<T, Ts...>;

template <typename T>
concept IsString = traits::is_string_v<T>;

template <typename T>
concept Stringifiable = requires(T t) {
    requires requires {
        { t.toString() } -> IsString;
    } || requires {
        { t.to_string() } -> IsString;
    };
};

template <typename T>
concept IsExpected = requires(T e) {
    typename std::remove_cvref_t<T>::value_type;
    typename std::remove_cvref_t<T>::error_type;
    typename std::remove_cvref_t<T>::unexpected_type;
    e.has_value();
    e.error();
    requires std::is_same_v<void, typename std::remove_cvref_t<T>::value_type> || requires(T e) { e.value(); };
};

template <typename T>
concept IsOptional = !IsExpected<T> && requires(T o) {
    o.value();
    o.has_value();
    o.operator*();
    typename std::remove_cvref_t<T>::value_type;
};

template <typename T>
concept IsRangeLoopable = std::is_bounded_array_v<std::remove_cvref_t<T>> || requires(T t) {
    t.begin();
    t.end();
} || requires(T t) {
    begin(t);
    end(t);
};

template <typename T>
concept IsAssociative = IsRangeLoopable<T> && requires {
    typename std::remove_cvref_t<T>::key_type;
    typename std::remove_cvref_t<T>::mapped_type;
};

template <typename T, template <typename...> typename Z>
concept IsSpecializes = traits::is_specialization_of_v<T, Z>;

template <typename T>
concept IsTupleLike = requires(T t) {
    std::tuple_size<std::remove_cvref_t<T>>::value;
    []<std::size_t... I>(T&& t, std::index_sequence<I...>) {
        ((void)std::get<I>(std::forward<T>(t)), ...);
    }(std::forward<T>(t), std::make_index_sequence<std::tuple_size<std::remove_cvref_t<T>>::value>{});
};

template <typename T>
concept IsArrayLike = IsRangeLoopable<T> && !requires { typename std::remove_cvref_t<T>::mapped_type; };

template <typename T>
concept IsVariant = requires { typename std::variant_size<std::remove_cvref_t<T>>::type; };

template <typename T>
concept IsDispatcher = requires(T t) {
    typename std::remove_cvref_t<T>::storage_type;
    typename std::remove_cvref_t<T>::listener_type;
    t.mStorage;
    t.call();
};

template <typename T>
concept Awaitable = traits::is_awaitable_v<T>;

} // namespace concepts

} // namespace jsonc_reflection