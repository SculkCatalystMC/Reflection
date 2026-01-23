#pragma once
#include "jsonc-reflection/detail/Concepts.hpp"
#include "jsonc-reflection/detail/Jsonc.hpp"
#include "jsonc-reflection/detail/Meta.hpp"
#include <boost/pfr.hpp>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

namespace jsonc_reflection {

namespace detail {

template <typename T, FixedString N>
inline jsonc::JsoncType serialize_impl(const Renamed<T, N>& t, bool& status, PriorityTag<7>) noexcept;

template <typename T, FixedString... Args>
inline jsonc::JsoncType serialize_impl(const Annotated<T, Args...>& t, bool& status, PriorityTag<6>) noexcept;

template <concepts::is_dispatcher T>
inline jsonc::JsoncType serialize_impl(const T& t, bool& status, PriorityTag<5>) noexcept;

template <concepts::is_optional T>
inline jsonc::JsoncType serialize_impl(const T& t, bool& status, PriorityTag<5>) noexcept;

template <concepts::is_string_type T>
inline jsonc::JsoncType serialize_impl(const T& t, bool&, PriorityTag<4>) noexcept;

template <concepts::is_ranged T>
inline jsonc::JsoncType serialize_impl(const T& t, bool&, PriorityTag<4>) noexcept;

template <concepts::is_aggregate T>
inline jsonc::JsoncType serialize_impl(const T& t, bool& status, PriorityTag<1>) noexcept;

template <concepts::is_enum T>
inline jsonc::JsoncType serialize_impl(const T& t, bool&, PriorityTag<1>) noexcept;

template <concepts::is_jsonc_type_convertible T>
inline jsonc::JsoncType serialize_impl(const T& t, bool&, PriorityTag<0>) noexcept;

} // namespace detail

template <concepts::is_reflectable T>
[[nodiscard]] inline jsonc::JsoncType serialize(const T& t, bool& status) noexcept {
    return detail::serialize_impl(t, status, PriorityTag<6>{});
}

template <concepts::is_reflectable T>
[[nodiscard]] inline jsonc::JsoncType serialize(const T& t) noexcept {
    bool status{};
    return detail::serialize_impl(t, status, PriorityTag<6>{});
}

namespace detail {

template <typename T, FixedString N>
inline jsonc::JsoncType serialize_impl(const Renamed<T, N>& t, bool& status, PriorityTag<7>) noexcept {
    return serialize_impl(*t, status, PriorityTag<6>{});
}

template <typename T, FixedString... Args>
inline jsonc::JsoncType serialize_impl(const Annotated<T, Args...>& t, bool& status, PriorityTag<6>) noexcept {
    auto result = serialize_impl(*t, status, PriorityTag<6>{});
    if (t.has_comments()) { result.set_before_comments(t.get_comments()); }
    return result;
}

template <concepts::is_dispatcher T>
inline jsonc::JsoncType serialize_impl(const T& t, bool& status, PriorityTag<5>) noexcept {
    return serialize_impl(*t, status, PriorityTag<5>{});
}

template <concepts::is_optional T>
inline jsonc::JsoncType serialize_impl(const T& t, bool& status, PriorityTag<5>) noexcept {
    if (t) { return serialize_impl(*t, status, PriorityTag<5>{}); }
    return nullptr;
}

template <concepts::is_string_type T>
inline jsonc::JsoncType serialize_impl(const T& t, bool&, PriorityTag<4>) noexcept {
    if constexpr (traits::is_string_convertible_v<T>) {
        return t;
    } else {
        if constexpr (traits::detail::has_to_string_v1<T>) {
            return t.to_string();
        } else if constexpr (traits::detail::has_to_string_v2<T>) {
            return t.toString();
        } else {
            return t.ToString();
        }
    }
}

template <concepts::is_ranged T>
inline jsonc::JsoncType serialize_impl(const T& t, bool& status, PriorityTag<4>) noexcept {
    return serialize_impl(*t, status, PriorityTag<4>{});
}

template <concepts::is_aggregate T>
inline jsonc::JsoncType serialize_impl(const T& t, bool& status, PriorityTag<1>) noexcept {
    auto result = jsonc::JsoncType::object();
    boost::pfr::for_each_field_with_name(t, [&](std::string_view name, const auto& val) {
        if constexpr (traits::is_renamed_v<decltype(val)>) { name = val.view(); }
        auto res = serialize_impl(val, status, PriorityTag<7>{});
        if (!res.is_null()) {
            result[name] = res;
            if (res.has_before_comments()) {
                result.set_key_before_comments(name, res.get_before_comments());
                result[name].clear_before_comments();
            }
        }
    });
    return result;
}

template <concepts::is_enum T>
inline jsonc::JsoncType serialize_impl(const T& t, bool&, PriorityTag<1>) noexcept {
    if (auto name = magic_enum::enum_name(t); !name.empty()) { return name; }
    if (auto flag = magic_enum::enum_flags_name(t); !flag.empty()) { return flag; }
    return std::to_underlying(t);
}

template <concepts::is_jsonc_type_convertible T>
inline jsonc::JsoncType serialize_impl(const T& t, bool&, PriorityTag<0>) noexcept {
    return t;
}

} // namespace detail

} // namespace jsonc_reflection


/*
namespace detail {



template <typename T>
inline jsonc::JsoncType serialize_impl(T&& tuple, PriorityTag<3>)
    requires(concepts::IsTupleLike<std::remove_cvref_t<T>>);
template <typename T>
inline jsonc::JsoncType serialize_impl(T&& tuple, PriorityTag<3>)
    requires(concepts::IsVariant<std::remove_cvref_t<T>>);
template <typename T>
inline jsonc::JsoncType serialize_impl(T&& arr, PriorityTag<2>)
    requires(concepts::IsArrayLike<std::remove_cvref_t<T>>);
template <typename T>
inline jsonc::JsoncType serialize_impl(T&& map, PriorityTag<2>)
    requires(concepts::IsAssociative<std::remove_cvref_t<T>>);

} // namespace detail

template <typename T>
[[nodiscard]] inline jsonc::JsoncType serialize(T&& t) noexcept {
    return detail::serialize_impl(std::forward<T>(t), PriorityTag<6>{});
}

namespace detail {

template <typename T>
[[nodiscard]] inline Expected<> serialize_to(jsonc::JsoncType& j, T&& t) noexcept {
    if (auto res = serialize(std::forward<T>(t)); res) {
        j = std::move(*res);
    } else {
        return forwardError(res.error());
    }
    return {};
}

template <typename T>
inline jsonc::JsoncType serialize_impl(T&& str, PriorityTag<4>)
    requires(concepts::IsString<std::remove_cvref_t<T>>)
{
    return jsonc::JsoncType(std::string{std::forward<T>(str)});
}
template <typename T>
inline jsonc::JsoncType serialize_impl(T&& num, PriorityTag<4>)
    requires(concepts::IsRanged<std::remove_cvref_t<T>>)
{
    return *num;
}
template <typename T>
inline jsonc::JsoncType serialize_impl(T&& tuple, PriorityTag<3>)
    requires(concepts::IsTupleLike<std::remove_cvref_t<T>>)
{
    jsonc::JsoncType res{jsonc::array()};
    std::apply(
        [&](auto&&... args) {
            size_t iter{0};
            (([&](auto&& arg) {
                 if (res) {
                     if (auto v = serialize(std::forward<decltype(arg)>(arg)); v) {
                         res->push_back(*std::move(v));
                         iter++;
                     } else {
                         res = makeSerIndexError(iter, v.error());
                     }
                 }
             }(std::forward<decltype(args)>(args))),
             ...);
        },
        std::forward<decltype(tuple)>(tuple)
    );
    return res;
}
template <typename T>
inline jsonc::JsoncType serialize_impl(T&& variant, PriorityTag<3>)
    requires(concepts::IsVariant<std::remove_cvref_t<T>>)
{
    jsonc::JsoncType res;
    std::visit([&](auto&& arg) { res = serialize(std::forward<decltype(arg)>(arg)); }, variant);
    return res;
}
template <typename T>
inline jsonc::JsoncType serialize_impl(T&& arr, PriorityTag<2>)
    requires(concepts::IsArrayLike<std::remove_cvref_t<T>>)
{
    jsonc::JsoncType res{jsonc::array()};
    size_t                     iter{0};
    for (auto&& val : std::forward<T>(arr)) {
        if (auto v = serialize(std::forward<decltype(val)>(val)); v) {
            res->push_back(*std::move(v));
            iter++;
        } else {
            res = makeStringError("Failed to serialize array {0}: {1}", iter, v.error());
            break;
        }
    }
    return res;
}
template <typename T>
inline jsonc::JsoncType serialize_impl(T&& map, PriorityTag<2>)
    requires(concepts::IsAssociative<std::remove_cvref_t<T>>)
{
    using RT = std::remove_cvref_t<T>;
    static_assert(
        (concepts::IsString<typename RT::key_type> || std::is_enum_v<typename RT::key_type>),
        "the key type of the associative container must be convertible to a string"
    );
    jsonc::JsoncType res{jsonc::object()};
    for (auto&& [k, v] : map) {
        std::string key;
        if constexpr (concepts::IsString<typename RT::key_type>) {
            key = std::string{std::forward<decltype(k)>(k)};
        } else {
            key = magic_enum::enum_name(std::forward<decltype(k)>(k));
        }
        if (auto sv = serialize(std::forward<decltype(v)>(v)); sv) {
            (*res)[key] = *std::move(sv);
        } else {
            res = makeStringError("Couldn't serialize menber {0}: {1}", key, sv.error());
            break;
        }
    }
    return res;
}

} // namespace detail

*/