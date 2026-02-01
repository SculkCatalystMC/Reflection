#pragma once
#include "jsonc/detail/reflection/concepts.hpp"
#include "jsonc/detail/reflection/jsonc_header.hpp"
#include "jsonc/detail/reflection/options.hpp"
#include "jsonc/detail/reflection/pfr.hpp"
#include "jsonc/detail/reflection/priority_tag.hpp"
#include "jsonc/detail/reflection/serializer.hpp"
#include "jsonc/detail/reflection/string_utils.hpp"

namespace jsonc::reflection {

namespace {

template <concepts::is_arithmetic T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<10>) noexcept;

template <concepts::is_boolean_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <concepts::is_signed_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <concepts::is_unsigned_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <concepts::is_float_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <concepts::is_string_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <concepts::is_object_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <concepts::is_array_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <concepts::is_big_int_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <concepts::is_renamed T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<8>) noexcept;

template <concepts::is_annotated T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<7>) noexcept;

template <concepts::is_dispatcher T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<6>) noexcept;

template <concepts::is_optional T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<6>) noexcept;

template <concepts::is_ranged T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<5>) noexcept;

template <concepts::is_enum T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<5>) noexcept;

template <concepts::is_variant T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<4>) noexcept;

template <concepts::is_string_convertible T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<3>) noexcept;

template <concepts::is_tuple_like T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<3>) noexcept;

template <concepts::is_array_like T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<2>) noexcept;

template <concepts::is_associative T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<2>) noexcept;

template <concepts::is_aggregate T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<1>) noexcept;

template <typename T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<0>) noexcept;

} // namespace

template <typename T>
[[nodiscard]] constexpr jsonc serialize(const T& t, const options& options = {}) noexcept {
    return serialize_impl(t, options, builtin_key_formatter::default_key_formatter, priority_tag<10>{});
}

template <typename T, concepts::is_key_formatter F>
[[nodiscard]] constexpr jsonc serialize(const T& t, const F& key_formatter, const options& options = {}) noexcept {
    return serialize_impl(t, options, key_formatter, priority_tag<10>{});
}

namespace {

template <concepts::is_arithmetic T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options&, const F&, priority_tag<10>) noexcept {
    return t;
}

template <concepts::is_boolean_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<bool>(serializer<T>::to_boolean(t));
}

template <concepts::is_signed_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<int64_t>(serializer<T>::to_signed(t));
}

template <concepts::is_unsigned_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<uint64_t>(serializer<T>::to_unsigned(t));
}

template <concepts::is_float_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    auto res = serializer<T>::to_float(t);
    if constexpr (std::same_as<float, std::remove_cvref_t<decltype(res)>>) {
        return static_cast<float>(res);
    } else {
        return static_cast<double>(res);
    }
}

template <concepts::is_string_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<std::string>(serializer<T>::to_string(t));
}

template <concepts::is_object_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<object_type>(serializer<T>::to_object(t));
}

template <concepts::is_array_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<array_type>(serializer<T>::to_array(t));
}

template <concepts::is_big_int_type T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return jsonc::from_big_int(static_cast<std::string>(serializer<T>::to_big_int(t))).value_or(0);
}

template <concepts::is_renamed T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<8>) noexcept {
    return serialize_impl(*t, options, kfmt, priority_tag<10>{});
}

template <concepts::is_annotated T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<7>) noexcept {
    auto result = serialize_impl(*t, options, kfmt, priority_tag<10>{});
    if (t.has_comments()) { result.set_before_comments(t.get_comments()); }
    return result;
}

template <concepts::is_dispatcher T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<6>) noexcept {
    return serialize_impl(*t, options, kfmt, priority_tag<10>{});
}

template <concepts::is_optional T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<6>) noexcept {
    if (t) { return serialize_impl(*t, options, kfmt, priority_tag<10>{}); }
    return nullptr;
}

template <concepts::is_ranged T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<5>) noexcept {
    return serialize_impl(*t, options, kfmt, priority_tag<10>{});
}

template <concepts::is_enum T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F&, priority_tag<5>) noexcept {
    if (options.enum_cast_prefer_string) {
        if (auto name = magic_enum::enum_name(t); !name.empty()) { return name; }
        if (auto flag = magic_enum::enum_flags_name(t); !flag.empty()) { return flag; }
    }
    return std::to_underlying(t);
}

template <concepts::is_variant T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<4>) noexcept {
    return std::visit([&](const auto& val) { return serialize_impl(val, options, kfmt, priority_tag<10>{}); }, t);
}

template <concepts::is_string_convertible T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options&, const F&, priority_tag<3>) noexcept {
    return static_cast<std::string>(t);
}

template <concepts::is_tuple_like T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<3>) noexcept {
    jsonc res = jsonc::array();
    std::apply(
        [&](const auto&... args) { (([&](const auto& v) { res.push_back(serialize_impl(v, options, kfmt, priority_tag<10>{})); }(args)), ...); },
        t
    );
    return res;
}

template <concepts::is_array_like T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<2>) noexcept {
    jsonc res = jsonc::array();
    for (const auto& val : t) { res.push_back(serialize_impl(val, options, kfmt, priority_tag<10>{})); }
    return res;
}

template <concepts::is_associative T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<2>) noexcept {
    static_assert(
        traits::is_stringifiable_type_v<typename std::remove_cvref_t<T>::key_type>,
        "the key type of the associative container must be convertible to a string"
    );
    jsonc res = jsonc::object();
    for (const auto& [key, val] : t) { res[detail::type_to_string(key)] = serialize_impl(val, options, kfmt, priority_tag<10>{}); }
    return res;
}

template <concepts::is_aggregate T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<1>) noexcept {
    auto result = jsonc::object();
    pfr::for_each_field_with_name(t, [&](std::string_view key, const auto& val) {
        std::string name = kfmt(key);
        if constexpr (traits::is_renamed_v<decltype(val)>) { name = val.view(); }
        auto res = serialize_impl(val, options, kfmt, priority_tag<10>{});
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

template <typename T, concepts::is_key_formatter F>
constexpr jsonc serialize_impl(const T&, const options&, const F&, priority_tag<0>) noexcept {
    static_assert(traits::always_false_v<T>, "type is not reflectable.");
}

} // namespace

} // namespace jsonc::reflection
