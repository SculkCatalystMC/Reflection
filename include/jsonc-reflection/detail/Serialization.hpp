#pragma once
#include "endgate/base/Concepts.hpp"
#include "endgate/base/Expected.hpp"
#include "endgate/base/Meta.hpp"
#include "endgate/reflection/Reflection.hpp"
#include <magic_enum/magic_enum.hpp>

namespace endgate::reflection {

namespace detail {
template <class J, class T>
inline Expected<J> serialize_impl(T&& obj, meta::PriorityTag<6>)
    requires(concepts::IsAnnotated<std::remove_cvref_t<T>>);
template <class J, class T>
inline Expected<J> serialize_impl(T&& d, meta::PriorityTag<5>)
    requires(concepts::IsDispatcher<std::remove_cvref_t<T>>);
template <class J, class T>
inline Expected<J> serialize_impl(T&& opt, meta::PriorityTag<5>)
    requires(concepts::IsOptional<std::remove_cvref_t<T>>);
template <class J, class T>
inline Expected<J> serialize_impl(T&& str, meta::PriorityTag<4>)
    requires(concepts::IsString<std::remove_cvref_t<T>>);
template <class J, class T>
inline Expected<J> serialize_impl(T&& str, meta::PriorityTag<4>)
    requires(concepts::IsRanged<std::remove_cvref_t<T>>);
template <class J, class T>
inline Expected<J> serialize_impl(T&& tuple, meta::PriorityTag<3>)
    requires(concepts::IsTupleLike<std::remove_cvref_t<T>>);
template <class J, class T>
inline Expected<J> serialize_impl(T&& tuple, meta::PriorityTag<3>)
    requires(concepts::IsVariant<std::remove_cvref_t<T>>);
template <class J, class T>
inline Expected<J> serialize_impl(T&& arr, meta::PriorityTag<2>)
    requires(concepts::IsArrayLike<std::remove_cvref_t<T>>);
template <class J, class T>
inline Expected<J> serialize_impl(T&& map, meta::PriorityTag<2>)
    requires(concepts::IsAssociative<std::remove_cvref_t<T>>);
template <class J, class T>
inline Expected<J> serialize_impl(T&& obj, meta::PriorityTag<1>)
    requires(concepts::IsReflectable<std::remove_cvref_t<T>>);
template <class J, class T>
inline Expected<J> serialize_impl(T&& e, meta::PriorityTag<1>)
    requires(std::is_enum_v<std::remove_cvref_t<T>>);
template <class J, class T>
inline Expected<J> serialize_impl(T&& obj, meta::PriorityTag<0>)
    requires(std::convertible_to<std::remove_cvref_t<T>, J>);
} // namespace detail

template <class J, class T>
[[nodiscard]] inline Expected<J> serialize(T&& t) try {
    return detail::serialize_impl<J>(std::forward<T>(t), meta::PriorityTag<6>{});
} catch (...) {
    return makeExceptionError();
}

namespace detail {
template <class J, class T>
[[nodiscard]] inline Expected<> serialize_to(J& j, T&& t) noexcept {
    if (auto res = serialize<J>(std::forward<T>(t)); res) {
        j = std::move(*res);
    } else {
        return forwardError(res.error());
    }
    return {};
}
template <class J, class T>
inline Expected<J> serialize_impl(T&& obj, meta::PriorityTag<6>)
    requires(concepts::IsAnnotated<std::remove_cvref_t<T>>)
{
    Expected<J> res;
    res = serialize<J>(std::forward<decltype(*obj)>(*obj));
    if constexpr (requires(J ins, jsonc::Node const& path, std::vector<std::string> const& comments) {
                      ins.setComments(path, comments);
                  }) {
        res->setComments({}, obj.getComments());
    }
    return res;
}
template <class J, class T>
inline Expected<J> serialize_impl(T&& d, meta::PriorityTag<5>)
    requires(concepts::IsDispatcher<std::remove_cvref_t<T>>)
{
    return serialize<J>(*std::forward<T>(d));
}
template <class J, class T>
inline Expected<J> serialize_impl(T&& opt, meta::PriorityTag<5>)
    requires(concepts::IsOptional<std::remove_cvref_t<T>>)
{
    if (!opt) {
        return nullptr;
    }
    return serialize<J>(*std::forward<T>(opt));
}
template <class J, class T>
inline Expected<J> serialize_impl(T&& str, meta::PriorityTag<4>)
    requires(concepts::IsString<std::remove_cvref_t<T>>)
{
    return std::string{std::forward<T>(str)};
}
template <class J, class T>
inline Expected<J> serialize_impl(T&& num, meta::PriorityTag<4>)
    requires(concepts::IsRanged<std::remove_cvref_t<T>>)
{
    return *num;
}
template <class J, class T>
inline Expected<J> serialize_impl(T&& tuple, meta::PriorityTag<3>)
    requires(concepts::IsTupleLike<std::remove_cvref_t<T>>)
{
    Expected<J> res{J::array()};
    std::apply(
        [&](auto&&... args) {
            size_t iter{0};
            (([&](auto&& arg) {
                 if (res) {
                     if (auto v = serialize<J>(std::forward<decltype(arg)>(arg)); v) {
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
template <class J, class T>
inline Expected<J> serialize_impl(T&& variant, meta::PriorityTag<3>)
    requires(concepts::IsVariant<std::remove_cvref_t<T>>)
{
    Expected<J> res;
    std::visit([&](auto&& arg) { res = serialize<J>(std::forward<decltype(arg)>(arg)); }, variant);
    return res;
}
template <class J, class T>
inline Expected<J> serialize_impl(T&& arr, meta::PriorityTag<2>)
    requires(concepts::IsArrayLike<std::remove_cvref_t<T>>)
{
    Expected<J> res{J::array()};
    size_t      iter{0};
    for (auto&& val : std::forward<T>(arr)) {
        if (auto v = serialize<J>(std::forward<decltype(val)>(val)); v) {
            res->push_back(*std::move(v));
            iter++;
        } else {
            res = makeStringError("Failed to serialize array {0}: {1}", iter, v.error());
            break;
        }
    }
    return res;
}
template <class J, class T>
inline Expected<J> serialize_impl(T&& map, meta::PriorityTag<2>)
    requires(concepts::IsAssociative<std::remove_cvref_t<T>>)
{
    using RT = std::remove_cvref_t<T>;
    static_assert(
        (concepts::IsString<typename RT::key_type> || std::is_enum_v<typename RT::key_type>),
        "the key type of the associative container must be convertible to a string"
    );
    Expected<J> res{J::object()};
    for (auto&& [k, v] : map) {
        std::string key;
        if constexpr (concepts::IsString<typename RT::key_type>) {
            key = std::string{std::forward<decltype(k)>(k)};
        } else {
            key = magic_enum::enum_name(std::forward<decltype(k)>(k));
        }
        if (auto sv = serialize<J>(std::forward<decltype(v)>(v)); sv) {
            (*res)[key] = *std::move(sv);
        } else {
            res = makeStringError("Couldn't serialize menber {0}: {1}", key, sv.error());
            break;
        }
    }
    return res;
}
template <class J, class T>
inline Expected<J> serialize_impl(T&& obj, meta::PriorityTag<1>)
    requires(concepts::IsReflectable<std::remove_cvref_t<T>>)
{
    Expected<J> res{J::object()};
    forEachFieldWithName(obj, [&](std::string_view name, auto const& member) {
        using MemberType = decltype(member);
        if constexpr (requires(MemberType m) { serialize<J>(m); }) {
            auto v = serialize<J>(std::forward<MemberType>(member));
            if (v) {
                if (!v->is_null()) (*res)[std::string{name}] = *std::move(v);
            } else {
                res = makeStringError("Couldn't serialize menber {0}: {1}", name, v.error());
            }
        } else {
            static_assert(traits::always_false<MemberType>, "this type can't serialize");
        }
    });
    return res;
}
template <class J, class T>
inline Expected<J> serialize_impl(T&& e, meta::PriorityTag<1>)
    requires(std::is_enum_v<std::remove_cvref_t<T>>)
{
    using enum_type = std::remove_cvref_t<T>;
    if constexpr (magic_enum::detail::supported<enum_type>::value) {
        if constexpr (magic_enum::detail::subtype_v<enum_type> == magic_enum::detail::enum_subtype::flags) {
            if (const auto name = magic_enum::enum_type_name<enum_type>(e); !name.empty()) {
                return name;
            }
        } else {
            if (const auto name = magic_enum::enum_name<enum_type>(e); !name.empty()) {
                return name;
            }
        }
    }
    return (std::underlying_type_t<enum_type>)e;
}
template <class J, class T>
inline Expected<J> serialize_impl(T&& obj, meta::PriorityTag<0>)
    requires(std::convertible_to<std::remove_cvref_t<T>, J>)
{
    return std::forward<T>(obj);
}
} // namespace detail

} // namespace endgate::reflection