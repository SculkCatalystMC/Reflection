#pragma once
#include "jsonc/detail/reflection/concepts.hpp"
#include "jsonc/detail/reflection/jsonc_header.hpp"
#include "jsonc/detail/reflection/options.hpp"
#include "jsonc/detail/reflection/pfr.hpp"
#include "jsonc/detail/reflection/priority_tag.hpp"
#include "jsonc/detail/reflection/string_utils.hpp"
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

namespace jsonc::reflection {

namespace {

template <concepts::is_arithmetic T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<10>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_boolean_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_signed_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_unsigned_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_float_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_string_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_object_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_array_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_high_precision_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<9>) noexcept;

template <concepts::is_renamed T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<8>) noexcept;

template <concepts::is_annotated T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<7>) noexcept;

template <concepts::is_dispatcher T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<6>) noexcept;

template <concepts::is_optional T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<6>) noexcept;

template <concepts::is_ranged T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<5>) noexcept;

template <concepts::is_enum T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<5>) noexcept;

template <concepts::is_variant T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<4>) noexcept;

template <concepts::is_string_convertible T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<3>) noexcept;

template <concepts::is_tuple_like T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<3>) noexcept;

template <concepts::is_array_like T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<2>) noexcept;

template <concepts::is_associative T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<2>) noexcept;

template <concepts::is_aggregate T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<1>) noexcept;

template <typename T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<0>) noexcept;

} // namespace

template <bool IsOrdered = true, bool AllowComments = true, typename T, concepts::is_key_formatter F>
[[nodiscard]] constexpr bool
deserialize(T& t, const detail::basic_jsonc<IsOrdered, AllowComments>& j, const F& key_formatter, const options& options = {}) noexcept {
    return deserialize_impl(t, j, options, key_formatter, priority_tag<10>{});
}

template <bool IsOrdered = true, bool AllowComments = true, typename T>
[[nodiscard]] constexpr bool deserialize(T& t, const detail::basic_jsonc<IsOrdered, AllowComments>& j, const options& options = {}) noexcept {
    return deserialize_impl(t, j, options, builtin_key_formatter::default_key_formatter, priority_tag<10>{});
}

namespace {

namespace {

template <concepts::is_arithmetic T, bool O, bool A>
constexpr bool deserialize_arithmetic_force_match(T& t, const detail::basic_jsonc<O, A>& j, const options&) noexcept {
    using RT = std::remove_cvref_t<T>;
    if constexpr (std::same_as<RT, bool>) {
        if (j.is_boolean()) {
            t = j.template get<bool>();
            return true;
        }
    } else if constexpr (std::is_floating_point_v<RT>) {
        if (j.is_number_float()) {
            t = static_cast<RT>(j.template get<double>());
            return true;
        }
    } else if constexpr (std::is_signed_v<RT>) {
        if (j.is_number_signed() || j.is_number_big_inteager()) {
            t = static_cast<RT>(j.template get<std::int64_t>());
            return true;
        }
    } else {
        if (j.is_number_unsigned() || j.is_number_big_inteager()) {
            t = static_cast<RT>(j.template get<std::uint64_t>());
            return true;
        }
    }
    return false;
}

template <concepts::is_variant T, typename F>
constexpr void for_each_type_in_variant(F&& f) {
    [&]<typename... Ts>(std::type_identity<std::variant<Ts...>>) {
        (f.template operator()<Ts>(), ...);
    }(std::type_identity<std::remove_cvref_t<T>>{});
}

template <bool O>
inline std::vector<std::string> extract_key_comments(const detail::basic_jsonc<O, true>& j, std::string_view key) noexcept {
    std::vector<std::string> result{};
    if (j.has_key_before_comments(key)) { result.append_range(j.get_key_before_comments(key)); }
    if (j.has_key_after_comments(key)) { result.append_range(j.get_key_after_comments(key)); }
    result.append_range(j[key].get_before_comments());
    result.append_range(j[key].get_after_comments());
    return result;
}

template <bool O>
inline std::vector<std::string> extract_value_comments(const detail::basic_jsonc<O, true>& j) noexcept {
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

} // namespace

template <concepts::is_arithmetic T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, const F&, priority_tag<10>) noexcept {
    using RT = std::remove_cvref_t<T>;
    if constexpr (std::same_as<RT, bool>) {
        if (j.is_boolean()) {
            t = j.template get<bool>();
            return true;
        }
    } else {
        if (j.is_number()) {
            t = j.template get<RT>();
            return true;
        }
    }
    return false;
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_boolean_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, const F&, priority_tag<9>) noexcept {
    if (!j.is_boolean()) { return false; }
    using AT = traits::serializer_arg_t<decltype(&serializer<T>::from_boolean)>;
    auto res = serializer<T>::from_boolean(static_cast<AT>(j.template get<bool>()));
    if (res) { t = *res; }
    return res.has_value();
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_signed_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, const F&, priority_tag<9>) noexcept {
    if (!j.is_number_signed()) { return false; }
    using AT = traits::serializer_arg_t<decltype(&serializer<T>::from_signed)>;
    auto res = serializer<T>::from_signed(static_cast<AT>(j.template get<std::int64_t>()));
    if (res) { t = *res; }
    return res.has_value();
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_unsigned_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, const F&, priority_tag<9>) noexcept {
    if (!j.is_number_unsigned()) { return false; }
    using AT = traits::serializer_arg_t<decltype(&serializer<T>::from_unsigned)>;
    auto res = serializer<T>::from_unsigned(static_cast<AT>(j.template get<std::uint64_t>()));
    if (res) { t = *res; }
    return res.has_value();
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_float_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, const F&, priority_tag<9>) noexcept {
    if (!j.is_number_float()) { return false; }
    using AT = traits::serializer_arg_t<decltype(&serializer<T>::from_float)>;
    auto res = serializer<T>::from_float(static_cast<AT>(j.template get<double>()));
    if (res) { t = *res; }
    return res.has_value();
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_string_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, const F&, priority_tag<9>) noexcept {
    if (!j.is_string()) { return false; }
    using AT = traits::serializer_arg_t<decltype(&serializer<T>::from_string)>;
    auto res = serializer<T>::from_string(static_cast<AT>(j.template as<std::string>()));
    if (res) { t = *res; }
    return res.has_value();
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_object_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, const F&, priority_tag<9>) noexcept {
    if (!j.is_object()) { return false; }
    auto res = serializer<T>::from_object(j.template as<typename detail::basic_jsonc<O, A>::object_type>());
    if (res) { t = *res; }
    return res.has_value();
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_array_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, const F&, priority_tag<9>) noexcept {
    if (!j.is_array()) { return false; }
    auto res = serializer<T>::from_array(j.template as<typename detail::basic_jsonc<O, A>::array_type>());
    if (res) { t = *res; }
    return res.has_value();
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
    requires traits::is_high_precision_type_v<T, O, A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, const F&, priority_tag<9>) noexcept {
    if (!j.is_number()) { return false; }
    auto res = serializer<T>::from_any_number(j.get_any_number_view());
    if (res.has_value()) { t = *res; }
    return res.has_value();
}

template <concepts::is_renamed T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<8>) noexcept {
    return deserialize_impl(*t, j, options, kfmt, priority_tag<10>{});
}

template <concepts::is_annotated T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<7>) noexcept {
    if constexpr (A) { t.set_comments(extract_value_comments(j)); }
    return deserialize_impl(*t, j, options, kfmt, priority_tag<10>{});
}

template <concepts::is_dispatcher T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<6>) noexcept {
    auto res = deserialize_impl(*t, j, options, kfmt, priority_tag<10>{});
    t.call();
    return res;
}

template <concepts::is_optional T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<6>) noexcept {
    if (!j.is_null()) {
        t.emplace();
        return deserialize_impl(*t, j, options, kfmt, priority_tag<10>{});
    } else {
        t = std::nullopt;
        return true;
    }
}

template <concepts::is_ranged T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<5>) noexcept {
    auto result = deserialize_impl(*t, j, options, kfmt, priority_tag<10>{});
    t.fix_range();
    return result;
}

template <concepts::is_enum T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, const F&, priority_tag<5>) noexcept {
    using RT = std::remove_cvref_t<T>;
    if (j.is_string()) {
        auto str = j.template get<std::string>();
        if (auto val = magic_enum::enum_cast<RT>(str)) {
            t = *val;
            return true;
        }
        if (auto val = magic_enum::enum_flags_cast<RT>(str)) {
            t = *val;
            return true;
        }
    } else if (j.is_number_signed()) {
        t = static_cast<RT>(j.template get<std::int64_t>());
        return true;
    } else if (j.is_number_unsigned()) {
        t = static_cast<RT>(j.template get<std::uint64_t>());
        return true;
    }
    return false;
}

template <concepts::is_variant T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<4>) noexcept {
    bool result{false};
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
            result = deserialize_impl(temp, j, options, kfmt, priority_tag<10>{});
            if (result) { t = std::move(temp); }
        }
    });
    return result;
}

template <concepts::is_string_convertible T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options&, const F&, priority_tag<3>) noexcept {
    if (j.is_string()) {
        t = j.template get<std::string>();
        return true;
    }
    return false;
}

template <concepts::is_tuple_like T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<3>) noexcept {
    if (!j.is_array()) { return false; }
    if (j.size() != std::tuple_size_v<T>) { return false; }
    bool result{true};
    std::apply(
        [&](auto&... args) {
            std::size_t index{0};
            (([&](auto& arg) {
                 result &= deserialize_impl(arg, j[index], options, kfmt, priority_tag<10>{});
                 index++;
             }(args)),
             ...);
        },
        t
    );
    return result;
}

template <concepts::is_array_like T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<2>) noexcept {
    if (!j.is_array()) { return false; }
    t.clear();
    bool result{true};
    for (std::size_t i = 0; i < j.size(); i++) {
        if constexpr (traits::has_emplace_back_method_v<T>) {
            result &= deserialize_impl(t.emplace_back(), j[i], options, kfmt, priority_tag<10>{});
        } else {
            typename std::remove_cvref_t<T>::value_type temp{};
            result &= deserialize_impl(temp, j[i], options, kfmt, priority_tag<10>{});
            t.insert(std::move(temp));
        }
    }
    return result;
}

template <concepts::is_associative T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<2>) noexcept {
    using KT = typename std::remove_cvref_t<T>::key_type;
    static_assert(traits::is_stringifiable_type_v<KT, O, A>, "the key type of the associative container must be convertible to a string");

    if (!j.is_object()) { return false; }
    bool result{true};
    t.clear();
    for (const auto& [k, v] : j.items()) {
        if (auto kt = detail::string_to_type<KT>(k)) {
            result &= deserialize_impl(t[*kt], v, options, kfmt, priority_tag<10>{});
            continue;
        }
        result &= false;
    }
    return result;
}

template <concepts::is_aggregate T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T& t, const detail::basic_jsonc<O, A>& j, const options& options, const F& kfmt, priority_tag<1>) noexcept {
    if (j.is_object()) {
        bool result{true};
        pfr::for_each_field_with_name(t, [&](std::string_view key, auto& val) {
            std::string name = kfmt(key);
            if constexpr (traits::is_renamed_v<decltype(val)>) { name = val.view(); }
            if (j.contains(name)) {
                result &= deserialize_impl(val, j[name], options, kfmt, priority_tag<10>{});
                if constexpr (A) { apply_key_comments(val, j, name, priority_tag<2>{}); }
            } else {
                deserialize_impl(val, detail::basic_jsonc<O, A>{}, options, kfmt, priority_tag<10>{});
            }
        });
        return result;
    } else {
        boost::pfr::for_each_field(t, [&](auto& val) { deserialize_impl(val, detail::basic_jsonc<O, A>{}, options, kfmt, priority_tag<10>{}); });
    }
    return false;
}

template <typename T, concepts::is_key_formatter F, bool O, bool A>
constexpr bool deserialize_impl(T&, const detail::basic_jsonc<O, A>&, const options&, const F&, priority_tag<0>) noexcept {
    static_assert(traits::always_false_v<T>, "type is not reflectable.");
}

} // namespace

} // namespace jsonc::reflection
