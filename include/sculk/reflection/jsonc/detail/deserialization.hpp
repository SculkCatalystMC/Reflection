// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/reflection/detail/pfr.hpp"
#include "sculk/reflection/detail/priority_tag.hpp"
#include "sculk/reflection/jsonc/detail/concepts.hpp"
#include "sculk/reflection/jsonc/detail/jsonc_header.hpp"
#include "sculk/reflection/jsonc/detail/options.hpp"
#include "sculk/reflection/utils/string_utils.hpp"
#include <expected>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

namespace sculk::reflection::jsonc {

namespace {

template <reflection::concepts::is_arithmetic T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<10>);

template <concepts::is_jsonc_array T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<9>);

template <concepts::is_jsonc_object T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<9>);

template <concepts::is_jsonc_variant T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<9>);

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_boolean_type_v<T, O, A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<9>);

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_signed_type_v<T, O, A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<9>);

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_unsigned_type_v<T, O, A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<9>);

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_float_type_v<T, O, A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<9>);

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_string_type_v<T, O, A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<9>);

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_object_type_v<T, O, A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<9>);

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_array_type_v<T, O, A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<9>);

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_high_precision_type_v<T, O, A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<9>);

template <concepts::is_renamed T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<8>);

template <concepts::is_annotated T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<7>);

template <concepts::is_dispatcher T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<6>);

template <concepts::is_optional T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<6>);

template <concepts::is_ranged T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<5>);

template <reflection::concepts::is_enum T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<5>);

template <concepts::is_variant T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<4>);

template <concepts::is_string_convertible T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<3>);

template <concepts::is_tuple_like T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<3>);

template <concepts::is_array_like T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<2>);

template <concepts::is_associative T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<2>);

template <concepts::is_aggregate T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<1>);

template <typename T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<0>);

} // namespace

template <bool IsOrdered = true, bool AllowComments = true, typename T, concepts::is_key_formatter F>
[[nodiscard]] constexpr std::expected<void, std::string>
deserialize(T& t, const detail::basic_jsonc<IsOrdered, AllowComments>& j, F&& key_formatter, const options& options = {}) {
    return deserialize_impl(t, j, options, std::forward<F>(key_formatter), priority_tag<10>{});
}

template <bool IsOrdered = true, bool AllowComments = true, typename T>
[[nodiscard]] constexpr std::expected<void, std::string>
deserialize(T& t, const detail::basic_jsonc<IsOrdered, AllowComments>& j, const options& options = {}) {
    return deserialize_impl(t, j, options, builtin_key_formatter::default_key_formatter, priority_tag<10>{});
}

namespace {

namespace {

template <reflection::concepts::is_arithmetic T, bool O, bool A>
constexpr std::expected<void, std::string> deserialize_arithmetic_force_match(T& t, const detail::basic_jsonc<O, A>& j, const options&) {
    using RT = std::remove_cvref_t<T>;
    if constexpr (std::same_as<RT, bool>) {
        if (j.is_boolean()) {
            t = j.template get<bool>();
            return {};
        }
    } else if constexpr (std::is_floating_point_v<RT>) {
        if (j.is_number_float()) {
            t = static_cast<RT>(j.template get<long double>());
            return {};
        }
    } else if constexpr (std::is_signed_v<RT>) {
        if (j.is_number_signed() || j.is_number_big_inteager()) {
            t = static_cast<RT>(j.template get<std::int64_t>());
            return {};
        }
    } else {
        if (j.is_number_unsigned() || j.is_number_big_inteager()) {
            t = static_cast<RT>(j.template get<std::uint64_t>());
            return {};
        }
    }
    return std::unexpected(std::format("type must be a number, but got {}", j.type_name()));
}

template <concepts::is_variant T, typename F>
constexpr void for_each_type_in_variant(F&& f) {
    [&]<typename... Ts>(std::type_identity<std::variant<Ts...>>) {
        (f.template operator()<Ts>(), ...);
    }(std::type_identity<std::remove_cvref_t<T>>{});
}

template <bool O>
inline std::vector<std::string> extract_key_comments(const detail::basic_jsonc<O, true>& j, std::string_view key) {
    std::vector<std::string> result{};
    if (j.has_key_before_comments(key)) { result.append_range(j.get_key_before_comments(key)); }
    if (j.has_key_after_comments(key)) { result.append_range(j.get_key_after_comments(key)); }
    result.append_range(j[key].get_before_comments());
    result.append_range(j[key].get_after_comments());
    return result;
}

template <bool O>
inline std::vector<std::string> extract_value_comments(const detail::basic_jsonc<O, true>& j) {
    std::vector<std::string> result{};
    result.append_range(j.get_before_comments());
    result.append_range(j.get_after_comments());
    return result;
}

template <concepts::is_renamed T, bool O>
constexpr void apply_key_comments(T& t, const detail::basic_jsonc<O, true>& j, std::string_view name, priority_tag<2>);

template <concepts::is_annotated T, bool O>
constexpr void apply_key_comments(T& t, const detail::basic_jsonc<O, true>& j, std::string_view name, priority_tag<1>);

template <concepts::is_renamed T, bool O>
constexpr void apply_key_comments(T& t, const detail::basic_jsonc<O, true>& j, std::string_view name, priority_tag<2>) {
    apply_key_comments(*t, j, name, priority_tag<2>{});
}

template <concepts::is_annotated T, bool O>
constexpr void apply_key_comments(T& t, const detail::basic_jsonc<O, true>& j, std::string_view name, priority_tag<1>) {
    t.set_comments(extract_key_comments(j, name));
}

template <typename T, bool O>
constexpr void apply_key_comments(T&, const detail::basic_jsonc<O, true>&, std::string_view, priority_tag<0>) {}

constexpr void forward_error_msg(std::expected<void, std::string>& result, std::string&& res) {
    if (result) {
        result = std::unexpected(std::move(res));
    } else {
        result.error().append("\n").append(std::move(res));
    }
}

} // namespace

template <reflection::concepts::is_arithmetic T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<10>) {
    using RT = std::remove_cvref_t<T>;
    if constexpr (std::same_as<RT, bool>) {
        if (j.is_boolean()) {
            t = j.template get<bool>();
            return {};
        }
    } else {
        if (j.is_number()) {
            t = j.template get<RT>();
            return {};
        }
    }
    return std::unexpected(std::format("type must be a number, but got {}", j.type_name()));
}

template <concepts::is_jsonc_array T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<9>) {
    if (j.is_array()) {
        for (const auto& e : j) { t.push_back(typename std::remove_cvref_t<T>::jsonc_type(e)); }
        return {};
    }
    return std::unexpected(std::format("type must be a array, but got {}", j.type_name()));
}

template <concepts::is_jsonc_object T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<9>) {
    if (j.is_object()) {
        for (const auto& [k, v] : j.items()) { t[k] = typename std::remove_cvref_t<T>::jsonc_type(v); }
        return {};
    }
    return std::unexpected(std::format("type must be a object, but got {}", j.type_name()));
}

template <concepts::is_jsonc_variant T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<9>) {
    t = static_cast<std::remove_cvref_t<T>>(j);
    return {};
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_boolean_type_v<T, O, A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<9>) {
    if (!j.is_boolean()) { return std::unexpected(std::format("type must be a boolean, but got {}", j.type_name())); }
    using AT = traits::serializer_arg_t<decltype(&serializer<T>::from_boolean)>;
    auto res = serializer<T>::from_boolean(static_cast<AT>(j.template get<bool>()));
    if (res) {
        t = *res;
        return {};
    }
    return std::unexpected(std::format("type must be a boolean, but got {}", j.type_name()));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_signed_type_v<T, O, A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<9>) {
    if (!j.is_number_integer()) { return std::unexpected(std::format("type must be a number, but got {}", j.type_name())); }
    using AT = traits::serializer_arg_t<decltype(&serializer<T>::from_signed)>;
    auto res = serializer<T>::from_signed(static_cast<AT>(j.template get<std::int64_t>()));
    if (res) {
        t = *res;
        return {};
    }
    return std::unexpected(std::format("type must be a number, but got {}", j.type_name()));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_unsigned_type_v<T, O, A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<9>) {
    if (!j.is_number_integer()) { return std::unexpected(std::format("type must be a number, but got {}", j.type_name())); }
    using AT = traits::serializer_arg_t<decltype(&serializer<T>::from_unsigned)>;
    auto res = serializer<T>::from_unsigned(static_cast<AT>(j.template get<std::uint64_t>()));
    if (res) {
        t = *res;
        return {};
    }
    return std::unexpected(std::format("type must be a number, but got {}", j.type_name()));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_float_type_v<T, O, A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<9>) {
    if (!j.is_number_float()) { return std::unexpected(std::format("type must be a number, but got {}", j.type_name())); }
    using AT = traits::serializer_arg_t<decltype(&serializer<T>::from_float)>;
    auto res = serializer<T>::from_float(static_cast<AT>(j.template get<long double>()));
    if (res) {
        t = *res;
        return {};
    }
    return std::unexpected(std::format("type must be a number, but got {}", j.type_name()));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_string_type_v<T, O, A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<9>) {
    if (!j.is_string()) { return std::unexpected(std::format("type must be a string, but got {}", j.type_name())); }
    using AT = traits::serializer_arg_t<decltype(&serializer<T>::from_string)>;
    auto res = serializer<T>::from_string(static_cast<AT>(j.template as<std::string>()));
    if (res) {
        t = *res;
        return {};
    }
    return std::unexpected(std::format("type must be a string, but got {}", j.type_name()));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_object_type_v<T, O, A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<9>) {
    if (!j.is_object()) { return std::unexpected(std::format("type must be a object, but got {}", j.type_name())); }
    auto res = serializer<T>::from_object(j.template as<typename detail::basic_jsonc<O, A>::object_type>());
    if (res) {
        t = *res;
        return {};
    }
    return std::unexpected(std::format("type must be a object, but got {}", j.type_name()));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_array_type_v<T, O, A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<9>) {
    if (!j.is_array()) { return std::unexpected(std::format("type must be a array, but got {}", j.type_name())); }
    auto res = serializer<T>::from_array(j.template as<typename detail::basic_jsonc<O, A>::array_type>());
    if (res) {
        t = *res;
        return {};
    }
    return std::unexpected(std::format("type must be a array, but got {}", j.type_name()));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_high_precision_type_v<T, O, A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<9>) {
    if (!j.is_number()) { return std::unexpected(std::format("type must be a number, but got {}", j.type_name())); }
    auto res = serializer<T>::from_any_number(j.get_any_number_view());
    if (res) {
        t = *res;
        return {};
    }
    return std::unexpected(std::format("type must be a number, but got {}", j.type_name()));
}

template <concepts::is_renamed T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<8>) {
    return deserialize_impl(*t, j, options, std::forward<F>(kfmt), priority_tag<10>{});
}

template <concepts::is_annotated T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<7>) {
    if constexpr (A) { t.set_comments(extract_value_comments(j)); }
    return deserialize_impl(*t, j, options, std::forward<F>(kfmt), priority_tag<10>{});
}

template <concepts::is_dispatcher T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<6>) {
    auto res = deserialize_impl(*t, j, options, std::forward<F>(kfmt), priority_tag<10>{});
    t.call();
    return res;
}

template <concepts::is_optional T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<6>) {
    if (!j.is_null()) {
        t.emplace();
        return deserialize_impl(*t, j, options, std::forward<F>(kfmt), priority_tag<10>{});
    } else {
        t = std::nullopt;
        return {};
    }
}

template <concepts::is_ranged T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<5>) {
    auto result = deserialize_impl(*t, j, options, std::forward<F>(kfmt), priority_tag<10>{});
    t.fix_range();
    return result;
}

template <reflection::concepts::is_enum T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&&, priority_tag<5>) {
    using RT = std::remove_cvref_t<T>;
    if (j.is_string()) {
        auto str = j.template get<std::string>();
        if (auto val = string_utils::str_to_enum<RT>(str, options.enum_cast_case_sensitive); val) {
            t = *val;
            return {};
        }
    } else if (j.is_number_signed()) {
        t = static_cast<RT>(j.template get<std::int64_t>());
        return {};
    } else if (j.is_number_unsigned()) {
        t = static_cast<RT>(j.template get<std::uint64_t>());
        return {};
    }
    return std::unexpected(std::format("type must be a number or string, but got {}", j.type_name()));
}

template <concepts::is_variant T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<4>) {
    std::expected<void, std::string> result =
        std::unexpected(std::format("could not cast target {} ({}) to any type in the variant", j.type_name(), j.dump()));
    for_each_type_in_variant<T>([&]<typename Ts> {
        if (!result) {
            if constexpr (std::is_arithmetic_v<Ts>) {
                Ts temp{};
                result = deserialize_arithmetic_force_match(temp, j, options);
                if (result) { t = std::move(temp); }
            }
        }
    });
    for_each_type_in_variant<T>([&]<typename Ts> {
        if (!result) {
            Ts temp{};
            result = deserialize_impl(temp, j, options, std::forward<F>(kfmt), priority_tag<10>{});
            if (result) { t = std::move(temp); }
        }
    });
    return result;
}

template <concepts::is_string_convertible T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string> deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, F&&, priority_tag<3>) {
    if (j.is_string()) {
        t = j.template get<std::string>();
        return {};
    }
    return std::unexpected(std::format("type must be a string, but got {}", j.type_name()));
}

template <concepts::is_tuple_like T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<3>) {
    if (!j.is_array()) { return std::unexpected(std::format("type must be an array, but got {}", j.type_name())); }
    if (j.size() != std::tuple_size_v<T>) {
        return std::unexpected(std::format("array size {} does not match tuple size {}", j.size(), std::tuple_size_v<T>));
    }
    std::expected<void, std::string> result{};
    std::apply(
        [&](auto&... args) {
            std::size_t index{0};
            (([&](auto& arg) {
                 auto res = deserialize_impl(arg, j[index], options, std::forward<F>(kfmt), priority_tag<10>{});
                 if (!res) { forward_error_msg(result, std::format("failed to deserialize tuple element at index {}: {}", index, res.error())); }
                 index++;
             }(args)),
             ...);
        },
        t
    );
    return result;
}

template <concepts::is_array_like T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<2>) {
    if (!j.is_array()) { return std::unexpected(std::format("type must be an array, but got {}", j.type_name())); }
    t.clear();
    std::expected<void, std::string> result{};
    for (std::size_t i = 0; i < j.size(); i++) {
        typename std::remove_cvref_t<T>::value_type temp{};

        auto res = deserialize_impl(temp, j[i], options, std::forward<F>(kfmt), priority_tag<10>{});
        if (res) {
            if constexpr (traits::has_push_back_method_v<T>) {
                t.push_back(std::move(temp));
            } else {
                t.insert(std::move(temp));
            }
        } else {
            forward_error_msg(result, std::format("failed to deserialize array element at index {}: {}", i, res.error()));
        }
    }
    return result;
}

template <concepts::is_associative T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<2>) {
    using KT = typename std::remove_cvref_t<T>::key_type;
    static_assert(traits::is_stringifiable_type_v<KT, O, A>, "the key type of the associative container must be convertible to a string");

    if (!j.is_object()) { return std::unexpected(std::format("type must be an object, but got {}", j.type_name())); }
    std::expected<void, std::string> result{};
    t.clear();
    for (const auto& [k, v] : j.items()) {
        if (auto kt = detail::string_to_type<KT>(k, options.enum_cast_case_sensitive)) {
            auto res = deserialize_impl(t[*kt], v, options, std::forward<F>(kfmt), priority_tag<10>{});
            if (!res) { forward_error_msg(result, std::format("failed to deserialize value for key '{}': {}", k, res.error())); }
            continue;
        }
        forward_error_msg(result, std::format("failed to convert key '{}' to the associative container key type", k));
    }
    return result;
}

template <concepts::is_aggregate T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string>
deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, F&& kfmt, priority_tag<1>) {
    if (j.is_object()) {
        std::expected<void, std::string> result{};
        pfr::for_each_field_with_name(t, [&](std::string_view key, auto& val) {
            auto name = std::invoke(kfmt, key);
            if constexpr (traits::is_renamed_v<decltype(val)>) { name = val.view(); }
            if (j.contains(name)) {
                auto res = deserialize_impl(val, j[name], options, kfmt, priority_tag<10>{});
                if (!res) { forward_error_msg(result, std::format("failed to deserialize field '{}': {}", name, res.error())); }
                if constexpr (A) { apply_key_comments(val, j, name, priority_tag<2>{}); }
            } else {
                if (options.check_missing_fields) {
                    if constexpr (!traits::is_optional_v<decltype(val)>) { forward_error_msg(result, std::format("missing field '{}'", name)); }
                } else {
                    (void)deserialize_impl(val, detail::basic_jsonc<O, A>{}, options, std::forward<F>(kfmt), priority_tag<10>{});
                }
            }
        });
        return result;
    } else {
        boost::pfr::for_each_field(t, [&](auto& val) {
            (void)deserialize_impl(val, detail::basic_jsonc<O, A>{}, options, std::forward<F>(kfmt), priority_tag<10>{});
        });
    }
    return std::unexpected(std::format("type must be an object, but got {}", j.type_name()));
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
constexpr std::expected<void, std::string> deserialize_impl(T&, const detail::basic_jsonc<O, A>&, const options&, F&&, priority_tag<0>) {
    static_assert(traits::always_false_v<T>, "type is not reflectable.");
    return std::unexpected("type is not reflectable.");
}

} // namespace

} // namespace sculk::reflection::jsonc
