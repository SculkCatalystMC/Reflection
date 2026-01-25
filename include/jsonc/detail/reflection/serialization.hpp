#pragma once
#include "jsonc/detail/reflection/concepts.hpp"
#include "jsonc/detail/reflection/jsonc_header.hpp"
#include "jsonc/detail/reflection/priority_tag.hpp"
#include <boost/pfr.hpp>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

namespace jsonc::reflection {

namespace detail {

template <typename T, FixedString N>
inline JsoncType serialize_impl(const Renamed<T, N>& t, PriorityTag<7>) noexcept;

template <typename T, FixedString... Args>
inline JsoncType serialize_impl(const Annotated<T, Args...>& t, PriorityTag<6>) noexcept;

template <concepts::is_dispatcher T>
inline JsoncType serialize_impl(const T& t, PriorityTag<5>) noexcept;

template <concepts::is_optional T>
inline JsoncType serialize_impl(const T& t, PriorityTag<5>) noexcept;

template <concepts::is_string_type T>
inline JsoncType serialize_impl(const T& t, PriorityTag<4>) noexcept;

template <concepts::is_ranged T>
inline JsoncType serialize_impl(const T& t, PriorityTag<4>) noexcept;

template <concepts::is_variant T>
inline JsoncType serialize_impl(const T& t, PriorityTag<3>) noexcept;

template <concepts::is_tuple_like T>
inline JsoncType serialize_impl(const T& t, PriorityTag<3>) noexcept;

template <concepts::is_array_like T>
inline JsoncType serialize_impl(const T& t, PriorityTag<2>) noexcept;

template <concepts::is_associative T>
inline JsoncType serialize_impl(const T& t, PriorityTag<2>) noexcept;

template <concepts::is_aggregate T>
inline JsoncType serialize_impl(const T& t, PriorityTag<1>) noexcept;

template <concepts::is_enum T>
inline JsoncType serialize_impl(const T& t, PriorityTag<1>) noexcept;

template <typename T>
inline JsoncType serialize_impl(const T& t, PriorityTag<0>) noexcept;

} // namespace detail

template <concepts::is_reflectable T>
[[nodiscard]] inline JsoncType serialize(const T& t) noexcept {
    return detail::serialize_impl(t, PriorityTag<6>{});
}

namespace detail {

template <typename T, FixedString N>
inline JsoncType serialize_impl(const Renamed<T, N>& t, PriorityTag<7>) noexcept {
    return serialize_impl(*t, PriorityTag<6>{});
}

template <typename T, FixedString... Args>
inline JsoncType serialize_impl(const Annotated<T, Args...>& t, PriorityTag<6>) noexcept {
    auto result = serialize_impl(*t, PriorityTag<6>{});
    if (t.has_comments()) { result.set_before_comments(t.get_comments()); }
    return result;
}

template <concepts::is_dispatcher T>
inline JsoncType serialize_impl(const T& t, PriorityTag<5>) noexcept {
    return serialize_impl(*t, PriorityTag<5>{});
}

template <concepts::is_optional T>
inline JsoncType serialize_impl(const T& t, PriorityTag<5>) noexcept {
    if (t) { return serialize_impl(*t, PriorityTag<5>{}); }
    return nullptr;
}

template <concepts::is_string_type T>
inline JsoncType serialize_impl(const T& t, PriorityTag<4>) noexcept {
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
inline JsoncType serialize_impl(const T& t, PriorityTag<4>) noexcept {
    return serialize_impl(*t, PriorityTag<4>{});
}

template <concepts::is_variant T>
inline JsoncType serialize_impl(const T& t, PriorityTag<3>) noexcept {
    return std::visit([&](const auto& val) { return serialize_impl(val, PriorityTag<7>{}); }, t);
}

template <concepts::is_tuple_like T>
inline JsoncType serialize_impl(const T& t, PriorityTag<3>) noexcept {
    JsoncType res = JsoncType::array();
    std::apply([&](const auto&... args) { (([&](const auto& arg) { res.push_back(serialize_impl(arg, PriorityTag<7>{})); }(args)), ...); }, t);
    return res;
}

template <concepts::is_array_like T>
inline JsoncType serialize_impl(const T& t, PriorityTag<2>) noexcept {
    JsoncType res = JsoncType::array();
    for (const auto& val : t) { res.push_back(serialize_impl(val, PriorityTag<7>{})); }
    return res;
}

template <concepts::is_associative T>
inline JsoncType serialize_impl(const T& t, PriorityTag<2>) noexcept {
    JsoncType res = JsoncType::object();
    for (const auto& [key, val] : t) {
        // TODO
    }
    return res;
}

template <concepts::is_aggregate T>
inline JsoncType serialize_impl(const T& t, PriorityTag<1>) noexcept {
    auto result = JsoncType::object();
    boost::pfr::for_each_field_with_name(t, [&](std::string_view name, const auto& val) {
        if constexpr (traits::is_renamed_v<decltype(val)>) { name = val.view(); }
        auto res = serialize_impl(val, PriorityTag<7>{});
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
inline JsoncType serialize_impl(const T& t, PriorityTag<1>) noexcept {
    if (auto name = magic_enum::enum_name(t); !name.empty()) { return name; }
    if (auto flag = magic_enum::enum_flags_name(t); !flag.empty()) { return flag; }
    return std::to_underlying(t);
}

template <typename T>
inline JsoncType serialize_impl(const T& t, PriorityTag<0>) noexcept {
    if constexpr (traits::is_jsonc_type_v<T>) {
        return t;
    } else {
        static_assert(traits::always_false_v<T>, "type is not reflectable.");
    }
}

} // namespace detail

} // namespace jsonc::reflection


/*

template <typename T>
inline JsoncType serialize_impl(T&& map, PriorityTag<2>)
    requires(concepts::IsAssociative<std::remove_cvref_t<T>>)
{
    using RT = std::remove_cvref_t<T>;
    static_assert(
        (concepts::IsString<typename RT::key_type> || std::is_enum_v<typename RT::key_type>),
        "the key type of the associative container must be convertible to a string"
    );
    JsoncType res{jsonc::object()};
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