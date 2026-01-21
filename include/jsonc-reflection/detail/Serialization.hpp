#pragma once
#include "jsonc-reflection/detail/Concepts.hpp"
#include "jsonc-reflection/detail/Expected.hpp"
#include "jsonc-reflection/detail/Meta.hpp"
#include "jsonc-reflection/detail/Reflection.hpp"
#include <jsonc/jsonc.hpp>
#include <magic_enum/magic_enum.hpp>

namespace jsonc_reflection {

namespace detail {
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& obj, PriorityTag<6>)
    requires(concepts::IsAnnotated<std::remove_cvref_t<T>>);
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& d, PriorityTag<5>)
    requires(concepts::IsDispatcher<std::remove_cvref_t<T>>);
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& opt, PriorityTag<5>)
    requires(concepts::IsOptional<std::remove_cvref_t<T>>);
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& str, PriorityTag<4>)
    requires(concepts::IsString<std::remove_cvref_t<T>>);
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& str, PriorityTag<4>)
    requires(concepts::IsRanged<std::remove_cvref_t<T>>);
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& tuple, PriorityTag<3>)
    requires(concepts::IsTupleLike<std::remove_cvref_t<T>>);
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& tuple, PriorityTag<3>)
    requires(concepts::IsVariant<std::remove_cvref_t<T>>);
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& arr, PriorityTag<2>)
    requires(concepts::IsArrayLike<std::remove_cvref_t<T>>);
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& map, PriorityTag<2>)
    requires(concepts::IsAssociative<std::remove_cvref_t<T>>);
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& obj, PriorityTag<1>)
    requires(concepts::IsReflectable<std::remove_cvref_t<T>>);
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& e, PriorityTag<1>)
    requires(std::is_enum_v<std::remove_cvref_t<T>>);
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& obj, PriorityTag<0>)
    requires(std::convertible_to<std::remove_cvref_t<T>, jsonc::JsoncType>);
} // namespace detail

template <typename T>
[[nodiscard]] inline Expected<jsonc::JsoncType> serialize(T&& t) noexcept {
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
inline Expected<jsonc::JsoncType> serialize_impl(T&& obj, PriorityTag<6>)
    requires(concepts::IsAnnotated<std::remove_cvref_t<T>>)
{
    Expected<jsonc::JsoncType> res;
    res = serialize(std::forward<decltype(*obj)>(*obj));
    return res;
}

template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& d, PriorityTag<5>)
    requires(concepts::IsDispatcher<std::remove_cvref_t<T>>)
{
    return serialize(*std::forward<T>(d));
}
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& opt, PriorityTag<5>)
    requires(concepts::IsOptional<std::remove_cvref_t<T>>)
{
    if (!opt) { return nullptr; }
    return serialize(*std::forward<T>(opt));
}
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& str, PriorityTag<4>)
    requires(concepts::IsString<std::remove_cvref_t<T>>)
{
    return jsonc::JsoncType(std::string{std::forward<T>(str)});
}
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& num, PriorityTag<4>)
    requires(concepts::IsRanged<std::remove_cvref_t<T>>)
{
    return *num;
}
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& tuple, PriorityTag<3>)
    requires(concepts::IsTupleLike<std::remove_cvref_t<T>>)
{
    Expected<jsonc::JsoncType> res{jsonc::array()};
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
inline Expected<jsonc::JsoncType> serialize_impl(T&& variant, PriorityTag<3>)
    requires(concepts::IsVariant<std::remove_cvref_t<T>>)
{
    Expected<jsonc::JsoncType> res;
    std::visit([&](auto&& arg) { res = serialize(std::forward<decltype(arg)>(arg)); }, variant);
    return res;
}
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& arr, PriorityTag<2>)
    requires(concepts::IsArrayLike<std::remove_cvref_t<T>>)
{
    Expected<jsonc::JsoncType> res{jsonc::array()};
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
inline Expected<jsonc::JsoncType> serialize_impl(T&& map, PriorityTag<2>)
    requires(concepts::IsAssociative<std::remove_cvref_t<T>>)
{
    using RT = std::remove_cvref_t<T>;
    static_assert(
        (concepts::IsString<typename RT::key_type> || std::is_enum_v<typename RT::key_type>),
        "the key type of the associative container must be convertible to a string"
    );
    Expected<jsonc::JsoncType> res{jsonc::object()};
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

template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& obj, PriorityTag<1>)
    requires(concepts::IsReflectable<std::remove_cvref_t<T>>)
{
    Expected<jsonc::JsoncType> res{jsonc::object()};
    forEachFieldWithName(obj, [&](std::string_view name, auto const& member) {
        using MemberType = decltype(member);
        if constexpr (requires(MemberType m) { serialize(m); }) {
            auto v = serialize(std::forward<MemberType>(member));
            if (v) {
                if (!v->is_null()) {
                    auto& root = res->as<jsonc::Object>();
                    root[name] = *std::move(v);
                }
            } else {
                res = makeStringError("Couldn't serialize menber {0}: {1}", name, v.error());
            }
        } else {
            static_assert(traits::always_false<MemberType>, "this type can't serialize");
        }
    });
    return res;
}
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& e, PriorityTag<1>)
    requires(std::is_enum_v<std::remove_cvref_t<T>>)
{
    using enum_type = std::remove_cvref_t<T>;
    if constexpr (magic_enum::detail::supported<enum_type>::value) {
        if constexpr (magic_enum::detail::subtype_v<enum_type> == magic_enum::detail::enum_subtype::flags) {
            if (const auto name = magic_enum::enum_type_name<enum_type>(e); !name.empty()) { return name; }
        } else {
            if (const auto name = magic_enum::enum_name<enum_type>(e); !name.empty()) { return name; }
        }
    }
    return (std::underlying_type_t<enum_type>)e;
}
template <typename T>
inline Expected<jsonc::JsoncType> serialize_impl(T&& obj, PriorityTag<0>)
    requires(std::convertible_to<std::remove_cvref_t<T>, jsonc::JsoncType>)
{
    return std::forward<T>(obj);
}
} // namespace detail

} // namespace jsonc_reflection