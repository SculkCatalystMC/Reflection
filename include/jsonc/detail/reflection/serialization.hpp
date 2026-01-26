#pragma once
#include "jsonc/detail/reflection/concepts.hpp"
#include "jsonc/detail/reflection/jsonc_header.hpp"
#include "jsonc/detail/reflection/options.hpp"
#include "jsonc/detail/reflection/priority_tag.hpp"
#include "jsonc/detail/reflection/serializer.hpp"
#include "jsonc/detail/reflection/string_utils.hpp"
#include <boost/pfr.hpp>

namespace jsonc::reflection {

namespace detail {

template <concepts::is_boolean_type T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_signed_type T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_unsigned_type T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_float_type T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_string_type T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_object_type T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_array_type T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_big_int_type T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<9>) noexcept;

template <typename T, FixedString N>
inline JsoncType serialize_impl(const Renamed<T, N>& t, const Options& options, PriorityTag<8>) noexcept;

template <typename T, FixedString... Args>
inline JsoncType serialize_impl(const Annotated<T, Args...>& t, const Options& options, PriorityTag<7>) noexcept;

template <concepts::is_dispatcher T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<6>) noexcept;

template <concepts::is_optional T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<6>) noexcept;

template <concepts::is_ranged T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<5>) noexcept;

template <concepts::is_arithmetic T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<5>) noexcept;

template <concepts::is_enum T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<5>) noexcept;

template <concepts::is_variant T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<4>) noexcept;

template <concepts::is_string_convertible T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<3>) noexcept;

template <concepts::is_tuple_like T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<3>) noexcept;

template <concepts::is_array_like T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<2>) noexcept;

template <concepts::is_associative T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<2>) noexcept;

template <concepts::is_aggregate T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<1>) noexcept;

template <typename T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<0>) noexcept;

} // namespace detail

template <typename T>
[[nodiscard]] inline JsoncType serialize(const T& t, const Options& options = {}) noexcept
    requires(
        traits::is_reflectable_v<std::remove_cvref_t<decltype(t)>>
        || (traits::is_reflectable_v<std::remove_cvref_t<decltype(*t)>> && traits::is_annotated_v<std::remove_cvref_t<decltype(t)>>)
    )
{
    auto result = detail::serialize_impl(*t, options, PriorityTag<9>{});
    result.set_before_comments(t.get_comments());
    return result;
}

namespace detail {

template <concepts::is_boolean_type T>
inline JsoncType serialize_impl(const T& t, const Options&, PriorityTag<9>) noexcept {
    return static_cast<bool>(Serializer<T>::to_boolean(t));
}

template <concepts::is_signed_type T>
inline JsoncType serialize_impl(const T& t, const Options&, PriorityTag<9>) noexcept {
    return static_cast<int64_t>(Serializer<T>::to_signed(t));
}

template <concepts::is_unsigned_type T>
inline JsoncType serialize_impl(const T& t, const Options&, PriorityTag<9>) noexcept {
    return static_cast<uint64_t>(Serializer<T>::to_unsigned(t));
}

template <concepts::is_float_type T>
inline JsoncType serialize_impl(const T& t, const Options&, PriorityTag<9>) noexcept {
    auto res = Serializer<T>::to_float(t);
    if constexpr (std::same_as<float, std::remove_cvref_t<decltype(res)>>) { return static_cast<float>(res); }
    return static_cast<double>(res);
}

template <concepts::is_string_type T>
inline JsoncType serialize_impl(const T& t, const Options&, PriorityTag<9>) noexcept {
    return static_cast<std::string>(Serializer<T>::to_string(t));
}

template <concepts::is_object_type T>
inline JsoncType serialize_impl(const T& t, const Options&, PriorityTag<9>) noexcept {
    return static_cast<Object>(Serializer<T>::to_object(t));
}

template <concepts::is_array_type T>
inline JsoncType serialize_impl(const T& t, const Options&, PriorityTag<9>) noexcept {
    return static_cast<Array>(Serializer<T>::to_array(t));
}

template <concepts::is_big_int_type T>
inline JsoncType serialize_impl(const T& t, const Options&, PriorityTag<9>) noexcept {
    return JsoncType::from_big_int(static_cast<std::string>(Serializer<T>::to_big_int(t)));
}

template <typename T, FixedString N>
inline JsoncType serialize_impl(const Renamed<T, N>& t, const Options& options, PriorityTag<8>) noexcept {
    return serialize_impl(*t, options, PriorityTag<9>{});
}

template <typename T, FixedString... Args>
inline JsoncType serialize_impl(const Annotated<T, Args...>& t, const Options& options, PriorityTag<7>) noexcept {
    auto result = serialize_impl(*t, options, PriorityTag<8>{});
    if (t.has_comments()) { result.set_before_comments(t.get_comments()); }
    return result;
}

template <concepts::is_dispatcher T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<6>) noexcept {
    return serialize_impl(*t, options, PriorityTag<7>{});
}

template <concepts::is_optional T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<6>) noexcept {
    if (t) { return serialize_impl(*t, options, PriorityTag<7>{}); }
    return nullptr;
}

template <concepts::is_ranged T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<5>) noexcept {
    return serialize_impl(*t, options, PriorityTag<5>{});
}

template <concepts::is_arithmetic T>
inline JsoncType serialize_impl(const T& t, const Options&, PriorityTag<5>) noexcept {
    return t;
}

template <concepts::is_enum T>
inline JsoncType serialize_impl(const T& t, const Options&, PriorityTag<5>) noexcept {
    if (auto name = magic_enum::enum_name(t); !name.empty()) { return name; }
    if (auto flag = magic_enum::enum_flags_name(t); !flag.empty()) { return flag; }
    return std::to_underlying(t);
}

template <concepts::is_variant T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<4>) noexcept {
    return std::visit([&](const auto& val) { return serialize_impl(val, options, PriorityTag<9>{}); }, t);
}

template <concepts::is_string_convertible T>
inline JsoncType serialize_impl(const T& t, const Options&, PriorityTag<3>) noexcept {
    return static_cast<std::string>(t);
}

template <concepts::is_tuple_like T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<3>) noexcept {
    JsoncType res = JsoncType::array();
    std::apply([&](const auto&... args) { (([&](const auto& v) { res.push_back(serialize_impl(v, options, PriorityTag<9>{})); }(args)), ...); }, t);
    return res;
}

template <concepts::is_array_like T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<2>) noexcept {
    JsoncType res = JsoncType::array();
    for (const auto& val : t) { res.push_back(serialize_impl(val, options, PriorityTag<9>{})); }
    return res;
}

template <concepts::is_associative T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<2>) noexcept {
    static_assert(
        traits::is_stringifiable_type_v<typename std::remove_cvref_t<T>::key_type>,
        "the key type of the associative container must be convertible to a string"
    );
    JsoncType res = JsoncType::object();
    for (const auto& [key, val] : t) { res[detail::string_utils::type_to_string(key)] = serialize_impl(val, options, PriorityTag<9>{}); }
    return res;
}

template <concepts::is_aggregate T>
inline JsoncType serialize_impl(const T& t, const Options& options, PriorityTag<1>) noexcept {
    auto result = JsoncType::object();
    boost::pfr::for_each_field_with_name(t, [&](std::string_view name, const auto& val) {
        if constexpr (traits::is_renamed_v<decltype(val)>) { name = val.view(); }
        auto res = serialize_impl(val, options, PriorityTag<9>{});
        if (!res.is_null() || options.keep_null) {
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
inline JsoncType serialize_impl(const T&, const Options&, PriorityTag<0>) noexcept {
    static_assert(traits::always_false_v<T>, "type is not reflectable.");
}

} // namespace detail

} // namespace jsonc::reflection
