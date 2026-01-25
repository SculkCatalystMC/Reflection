#pragma once
#include "jsonc/detail/reflection/concepts.hpp"
#include "jsonc/detail/reflection/jsonc_header.hpp"
#include "jsonc/detail/reflection/priority_tag.hpp"
#include "jsonc/detail/reflection/string_utils.hpp"
#include <boost/pfr.hpp>

namespace jsonc::reflection {

namespace detail {

template <typename T, FixedString N>
inline JsoncType serialize_impl(const Renamed<T, N>& t, PriorityTag<8>) noexcept;

template <typename T, FixedString... Args>
inline JsoncType serialize_impl(const Annotated<T, Args...>& t, PriorityTag<7>) noexcept;

template <concepts::is_dispatcher T>
inline JsoncType serialize_impl(const T& t, PriorityTag<6>) noexcept;

template <concepts::is_optional T>
inline JsoncType serialize_impl(const T& t, PriorityTag<6>) noexcept;

template <concepts::is_ranged T>
inline JsoncType serialize_impl(const T& t, PriorityTag<5>) noexcept;

template <concepts::is_arithmetic T>
inline JsoncType serialize_impl(const T& t, PriorityTag<5>) noexcept;

template <concepts::is_enum T>
inline JsoncType serialize_impl(const T& t, PriorityTag<5>) noexcept;

template <concepts::is_string_type T>
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

template <typename T>
inline JsoncType serialize_impl(const T& t, PriorityTag<0>) noexcept;

} // namespace detail

template <typename T>
[[nodiscard]] inline JsoncType serialize(const T& t) noexcept
    requires(
        traits::is_reflectable_v<std::remove_cvref_t<decltype(t)>>
        || (traits::is_reflectable_v<std::remove_cvref_t<decltype(*t)>> && traits::is_annotated_v<std::remove_cvref_t<decltype(t)>>)
    )
{
    auto result = detail::serialize_impl(*t, PriorityTag<8>{});
    result.set_before_comments(t.get_comments());
    return result;
}

namespace detail {

template <typename T, FixedString N>
inline JsoncType serialize_impl(const Renamed<T, N>& t, PriorityTag<8>) noexcept {
    return serialize_impl(*t, PriorityTag<7>{});
}

template <typename T, FixedString... Args>
inline JsoncType serialize_impl(const Annotated<T, Args...>& t, PriorityTag<7>) noexcept {
    auto result = serialize_impl(*t, PriorityTag<7>{});
    if (t.has_comments()) { result.set_before_comments(t.get_comments()); }
    return result;
}

template <concepts::is_dispatcher T>
inline JsoncType serialize_impl(const T& t, PriorityTag<6>) noexcept {
    return serialize_impl(*t, PriorityTag<6>{});
}

template <concepts::is_optional T>
inline JsoncType serialize_impl(const T& t, PriorityTag<6>) noexcept {
    if (t) { return serialize_impl(*t, PriorityTag<6>{}); }
    return nullptr;
}

template <concepts::is_ranged T>
inline JsoncType serialize_impl(const T& t, PriorityTag<5>) noexcept {
    return serialize_impl(*t, PriorityTag<5>{});
}

template <concepts::is_arithmetic T>
inline JsoncType serialize_impl(const T& t, PriorityTag<5>) noexcept {
    return t;
}

template <concepts::is_enum T>
inline JsoncType serialize_impl(const T& t, PriorityTag<5>) noexcept {
    if (auto name = magic_enum::enum_name(t); !name.empty()) { return name; }
    if (auto flag = magic_enum::enum_flags_name(t); !flag.empty()) { return flag; }
    return std::to_underlying(t);
}


template <concepts::is_string_type T>
inline JsoncType serialize_impl(const T& t, PriorityTag<4>) noexcept {
    if constexpr (traits::is_string_convertible_v<T>) {
        return t;
    } else {
        return Serializer<T>::to_string(t);
    }
}

template <concepts::is_variant T>
inline JsoncType serialize_impl(const T& t, PriorityTag<3>) noexcept {
    return std::visit([&](const auto& val) { return serialize_impl(val, PriorityTag<8>{}); }, t);
}

template <concepts::is_tuple_like T>
inline JsoncType serialize_impl(const T& t, PriorityTag<3>) noexcept {
    JsoncType res = JsoncType::array();
    std::apply([&](const auto&... args) { (([&](const auto& arg) { res.push_back(serialize_impl(arg, PriorityTag<8>{})); }(args)), ...); }, t);
    return res;
}

template <concepts::is_array_like T>
inline JsoncType serialize_impl(const T& t, PriorityTag<2>) noexcept {
    JsoncType res = JsoncType::array();
    for (const auto& val : t) { res.push_back(serialize_impl(val, PriorityTag<8>{})); }
    return res;
}

template <concepts::is_associative T>
inline JsoncType serialize_impl(const T& t, PriorityTag<2>) noexcept {
    using KT = typename std::remove_cvref_t<T>::key_type;
    // using VT = typename std::remove_cvref_t<T>::value_type;
    static_assert(traits::is_string_type_v<KT>, "the key type of the associative container must be convertible to a string");
    // static_assert(TODO , "The value type of the associative container must be serializable");
    JsoncType res = JsoncType::object();
    for (const auto& [key, val] : t) { res[string_utils::type_to_string(key)] = val; }
    return res;
}

template <concepts::is_aggregate T>
inline JsoncType serialize_impl(const T& t, PriorityTag<1>) noexcept {
    auto result = JsoncType::object();
    boost::pfr::for_each_field_with_name(t, [&](std::string_view name, const auto& val) {
        if constexpr (traits::is_renamed_v<decltype(val)>) { name = val.view(); }
        auto res = serialize_impl(val, PriorityTag<8>{});
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
