// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/reflection/detail/concepts.hpp"
#include "sculk/reflection/detail/pfr.hpp"
#include "sculk/reflection/detail/priority_tag.hpp"
#include "sculk/reflection/jsonc/detail/jsonc_header.hpp"
#include "sculk/reflection/jsonc/detail/options.hpp"
#include "sculk/reflection/jsonc/detail/serializer.hpp"
#include "sculk/reflection/utils/string_utils.hpp"

namespace sculk::reflection::jsonc {

namespace {

template <reflection::concepts::is_arithmetic T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<10>) noexcept;

template <concepts::is_jsonc_array T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <concepts::is_jsonc_object T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <concepts::is_jsonc_variant T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_boolean_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_signed_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_unsigned_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_float_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_string_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_object_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_array_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_high_precision_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <concepts::is_renamed T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<8>) noexcept;

template <concepts::is_annotated T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<7>) noexcept;

template <concepts::is_dispatcher T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<6>) noexcept;

template <concepts::is_optional T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<6>) noexcept;

template <concepts::is_ranged T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<5>) noexcept;

template <reflection::concepts::is_enum T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<5>) noexcept;

template <concepts::is_variant T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<4>) noexcept;

template <concepts::is_string_convertible T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<3>) noexcept;

template <concepts::is_tuple_like T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<3>) noexcept;

template <concepts::is_array_like T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<2>) noexcept;

template <concepts::is_associative T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<2>) noexcept;

template <concepts::is_aggregate T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<1>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<0>) noexcept;

} // namespace

template <bool IsOrdered = true, bool AllowComments = true, typename T>
[[nodiscard]] constexpr detail::basic_jsonc<IsOrdered, AllowComments> serialize(const T& t, const options& options = {}) noexcept {
    return serialize_impl(t, options, builtin_key_formatter::default_key_formatter, priority_tag<10>{});
}

template <bool IsOrdered = true, bool AllowComments = true, typename T, concepts::is_key_formatter F>
[[nodiscard]] constexpr detail::basic_jsonc<IsOrdered, AllowComments>
serialize(const T& t, const F& key_formatter, const options& options = {}) noexcept {
    return serialize_impl<T, F, IsOrdered, AllowComments>(t, options, key_formatter, priority_tag<10>{});
}

namespace {

template <reflection::concepts::is_arithmetic T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<10>) noexcept {
    return t;
}

template <concepts::is_jsonc_array T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    detail::basic_jsonc<O, A> res{};
    for (const auto& e : t) { res.push_back(detail::basic_jsonc<O, A>(e)); }
    return res;
}

template <concepts::is_jsonc_object T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    detail::basic_jsonc<O, A> res{};
    for (const auto& [k, v] : t) { res[k] = detail::basic_jsonc<O, A>(v); }
    return res;
}

template <concepts::is_jsonc_variant T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<detail::basic_jsonc<O, A>>(t);
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_boolean_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<bool>(serializer<T>::to_boolean(t));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_signed_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<std::int64_t>(serializer<T>::to_signed(t));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_unsigned_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<std::uint64_t>(serializer<T>::to_unsigned(t));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_float_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    auto res = serializer<T>::to_float(t);
    if constexpr (std::same_as<float, std::remove_cvref_t<decltype(res)>>) {
        return static_cast<float>(res);
    } else {
        return static_cast<double>(res);
    }
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_string_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<std::string>(serializer<T>::to_string(t));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_object_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<detail::basic_jsonc<O, A>::object_type>(serializer<T>::to_object(t));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_array_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return static_cast<detail::basic_jsonc<O, A>::array_type>(serializer<T>::to_array(t));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_high_precision_type_v<T, O, A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<9>) noexcept {
    return detail::basic_jsonc<O, A>::from_any_number(static_cast<std::string>(serializer<T>::to_any_number(t)), true).value_or(0);
}

template <concepts::is_renamed T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<8>) noexcept {
    using VT = std::remove_cvref_t<decltype(*t)>;
    return serialize_impl<VT, F, O, A>(*t, options, kfmt, priority_tag<10>{});
}

template <concepts::is_annotated T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<7>) noexcept {
    using VT    = std::remove_cvref_t<decltype(*t)>;
    auto result = serialize_impl<VT, F, O, A>(*t, options, kfmt, priority_tag<10>{});
    if constexpr (A) {
        if (t.has_comments()) { result.set_before_comments(t.get_comments()); }
    }
    return result;
}

template <concepts::is_dispatcher T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<6>) noexcept {
    using VT = std::remove_cvref_t<decltype(*t)>;
    return serialize_impl<VT, F, O, A>(*t, options, kfmt, priority_tag<10>{});
}

template <concepts::is_optional T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<6>) noexcept {
    if (t) {
        using VT = std::remove_cvref_t<decltype(*t)>;
        return serialize_impl<VT, F, O, A>(*t, options, kfmt, priority_tag<10>{});
    }
    return nullptr;
}

template <concepts::is_ranged T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<5>) noexcept {
    using VT = std::remove_cvref_t<decltype(*t)>;
    return serialize_impl<VT, F, O, A>(*t, options, kfmt, priority_tag<10>{});
}

template <reflection::concepts::is_enum T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F&, priority_tag<5>) noexcept {
    if (options.enum_cast_prefer_string) {
        if (auto name = magic_enum::enum_name(t); !name.empty()) { return name; }
        if (auto flag = magic_enum::enum_flags_name(t); !flag.empty()) { return flag; }
    }
    return std::to_underlying(t);
}

template <concepts::is_variant T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<4>) noexcept {
    return std::visit(
        [&](const auto& val) {
            using VT = std::remove_cvref_t<decltype(val)>;
            return serialize_impl<VT, F, O, A>(val, options, kfmt, priority_tag<10>{});
        },
        t
    );
}

template <concepts::is_string_convertible T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options&, const F&, priority_tag<3>) noexcept {
    return static_cast<std::string>(t);
}

template <concepts::is_tuple_like T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<3>) noexcept {
    detail::basic_jsonc<O, A> res = detail::basic_jsonc<O, A>::array();
    std::apply(
        [&](const auto&... args) {
            (([&](const auto& val) {
                 using VT = std::remove_cvref_t<decltype(val)>;
                 res.push_back(serialize_impl<VT, F, O, A>(val, options, kfmt, priority_tag<10>{}));
             }(args)),
             ...);
        },
        t
    );
    return res;
}

template <concepts::is_array_like T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<2>) noexcept {
    detail::basic_jsonc<O, A> res = detail::basic_jsonc<O, A>::array();
    for (const auto& val : t) {
        using VT = std::remove_cvref_t<decltype(val)>;
        res.push_back(serialize_impl<VT, F, O, A>(val, options, kfmt, priority_tag<10>{}));
    }
    return res;
}

template <concepts::is_associative T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<2>) noexcept {
    static_assert(
        traits::is_stringifiable_type_v<typename std::remove_cvref_t<T>::key_type, O, A>,
        "the key type of the associative container must be convertible to a string"
    );
    detail::basic_jsonc<O, A> res = detail::basic_jsonc<O, A>::object();
    for (const auto& [key, val] : t) {
        using VT                         = std::remove_cvref_t<decltype(val)>;
        res[detail::type_to_string(key)] = serialize_impl<VT, F, O, A>(val, options, kfmt, priority_tag<10>{});
    }
    return res;
}

template <concepts::is_aggregate T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T& t, const options& options, const F& kfmt, priority_tag<1>) noexcept {
    auto result = detail::basic_jsonc<O, A>::object();
    pfr::for_each_field_with_name(t, [&](std::string_view key, const auto& val) {
        std::string name = kfmt(key);
        using VT         = std::remove_cvref_t<decltype(val)>;
        if constexpr (traits::is_renamed_v<VT>) { name = val.view(); }
        auto res = serialize_impl<VT, F, O, A>(val, options, kfmt, priority_tag<10>{});
        if (!res.is_null() || options.keep_null) {
            result[name] = res;
            if constexpr (A) {
                if (res.has_before_comments()) {
                    result.set_key_before_comments(name, res.get_before_comments());
                    result[name].clear_before_comments();
                }
            }
        }
    });
    return result;
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
constexpr detail::basic_jsonc<O, A> serialize_impl(const T&, const options&, const F&, priority_tag<0>) noexcept {
    static_assert(traits::always_false_v<T>, "type is not reflectable.");
}

} // namespace

} // namespace sculk::reflection::jsonc
