#pragma once
#include "jsonc/detail/reflection/concepts.hpp"
#include "jsonc/detail/reflection/jsonc_header.hpp"
#include "jsonc/detail/reflection/options.hpp"
#include "jsonc/detail/reflection/priority_tag.hpp"
#include "jsonc/detail/reflection/string_utils.hpp"
#include <boost/pfr.hpp>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

namespace jsonc::reflection {

namespace {

template <concepts::is_arithmetic T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<10>) noexcept;

template <concepts::is_boolean_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_signed_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_unsigned_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_float_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_string_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_object_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_array_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<9>) noexcept;

template <concepts::is_big_int_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<9>) noexcept;

template <typename T, FixedString N>
inline bool deserialize_impl(Renamed<T, N>& t, const JsoncType& j, const Options& options, PriorityTag<8>) noexcept;

template <typename T, FixedString... Args>
inline bool deserialize_impl(Annotated<T, Args...>& t, const JsoncType& j, const Options& options, PriorityTag<7>) noexcept;

template <concepts::is_dispatcher T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<6>) noexcept;

template <concepts::is_optional T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<6>) noexcept;

template <concepts::is_ranged T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<5>) noexcept;

template <concepts::is_enum T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<5>) noexcept;

template <concepts::is_variant T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<4>) noexcept;

template <concepts::is_string_convertible T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<3>) noexcept;

template <concepts::is_tuple_like T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<3>) noexcept;

template <concepts::is_array_like T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<2>) noexcept;

template <concepts::is_associative T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<2>) noexcept;

template <concepts::is_aggregate T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<1>) noexcept;

template <typename T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<0>) noexcept;

} // namespace

template <typename T>
[[nodiscard]] inline bool deserialize(T& t, const JsoncType& j, const Options& options = {}) noexcept
    requires(
        traits::is_reflectable_v<std::remove_cvref_t<decltype(t)>>
        || (traits::is_reflectable_v<std::remove_cvref_t<decltype(*t)>> && traits::is_annotated_v<std::remove_cvref_t<decltype(t)>>)
    )
{
    if constexpr (traits::is_annotated_v<std::remove_cvref_t<decltype(t)>>) {
        bool result = deserialize_impl(*t, j, options, PriorityTag<10>{});
        t.set_comments(j.get_before_comments());
        return result;
    } else {
        return deserialize_impl(t, j, options, PriorityTag<10>{});
    }
}

namespace {

template <concepts::is_arithmetic T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<10>) noexcept {
    using RT = std::remove_cvref_t<T>;
    if constexpr (std::same_as<RT, bool>) {
        if (j.is_boolean()) {
            t = j.get<bool>();
            return true;
        }
    } else {
        if (j.is_number_integer() || j.is_number_float()) {
            t = j.get<RT>();
            return true;
        }
    }
    return false;
}

template <concepts::is_boolean_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<9>) noexcept {
    if (!j.is_boolean()) { return false; }
    auto res = Serializer<T>::from_boolean(j.get<bool>());
    if (res) { t = *res; }
    return res.has_value();
}

template <concepts::is_signed_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<9>) noexcept {
    if (!j.is_number_signed()) { return false; }
    auto res = Serializer<T>::from_signed(j.get<int64_t>());
    if (res) { t = *res; }
    return res.has_value();
}

template <concepts::is_unsigned_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<9>) noexcept {
    if (!j.is_number_unsigned()) { return false; }
    auto res = Serializer<T>::from_unsigned(j.get<uint64_t>());
    if (res) { t = *res; }
    return res.has_value();
}

template <concepts::is_float_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<9>) noexcept {
    if (!j.is_number_float()) { return false; }
    auto res = Serializer<T>::from_float(j.get<double>());
    if (res) { t = *res; }
    return res.has_value();
}

template <concepts::is_string_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<9>) noexcept {
    if (!j.is_string()) { return false; }
    auto res = Serializer<T>::from_string(j.as<std::string>());
    if (res) { t = *res; }
    return res.has_value();
}

template <concepts::is_object_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<9>) noexcept {
    if (!j.is_object()) { return false; }
    auto res = Serializer<T>::from_object(j.as<Object>());
    if (res) { t = *res; }
    return res.has_value();
}

template <concepts::is_array_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<9>) noexcept {
    if (!j.is_array()) { return false; }
    auto res = Serializer<T>::from_array(j.as<Array>());
    if (res) { t = *res; }
    return res.has_value();
}

template <concepts::is_big_int_type T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<9>) noexcept {
    if (!j.is_number_any_inteager()) { return false; }
    auto res = Serializer<T>::from_big_int(j.get_any_int_view());
    if (res.has_value()) { t = *res; }
    return res.has_value();
}

template <typename T, FixedString N>
inline bool deserialize_impl(Renamed<T, N>& t, const JsoncType& j, const Options& options, PriorityTag<8>) noexcept {
    return deserialize_impl(*t, j, options, PriorityTag<10>{});
}

template <typename T, FixedString... Args>
inline bool deserialize_impl(Annotated<T, Args...>& t, const JsoncType& j, const Options& options, PriorityTag<7>) noexcept {
    return deserialize_impl(*t, j, options, PriorityTag<10>{});
}

template <concepts::is_dispatcher T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<6>) noexcept {
    return deserialize_impl(*t, j, options, PriorityTag<10>{});
}

template <concepts::is_optional T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<6>) noexcept {
    if (!j.is_null()) {
        t.emplace();
        return deserialize_impl(*t, j, options, PriorityTag<10>{});
    } else {
        t = std::nullopt;
        return true;
    }
}

template <concepts::is_ranged T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<5>) noexcept {
    auto result = deserialize_impl(*t, j, options, PriorityTag<10>{});
    t.fix_range();
    return result;
}

template <concepts::is_enum T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<5>) noexcept {
    using RT = std::remove_cvref_t<T>;
    if (j.is_string()) {
        auto str = j.get<std::string>();
        if (auto val = magic_enum::enum_cast<RT>(str)) {
            t = *val;
            return true;
        }
        if (auto val = magic_enum::enum_flags_cast<RT>(str)) {
            t = *val;
            return true;
        }
    } else if (j.is_number_signed()) {
        t = static_cast<RT>(j.get<int64_t>());
        return true;
    } else if (j.is_number_unsigned()) {
        t = static_cast<RT>(j.get<uint64_t>());
        return true;
    }
    return false;
}

namespace {

template <concepts::is_arithmetic T>
inline bool deserialize_arithmetic_force_match(T& t, const JsoncType& j, const Options&) noexcept {
    using RT = std::remove_cvref_t<T>;
    if constexpr (std::same_as<RT, bool>) {
        if (j.is_boolean()) {
            t = j.get<bool>();
            return true;
        }
    } else if constexpr (std::is_floating_point_v<RT>) {
        if (j.is_number_float()) {
            t = j.get<double>();
            return true;
        }
    } else if constexpr (std::is_signed_v<RT>) {
        if (j.is_number_signed()) {
            t = j.get<int64_t>();
            return true;
        }
    } else {
        if (j.is_number_unsigned()) {
            t = j.get<uint64_t>();
            return true;
        }
    }
    return false;
}

} // namespace

template <concepts::is_variant T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<4>) noexcept {
    bool result{false};
    [&]<typename... Ts>(std::type_identity<std::variant<Ts...>>) {
        (
            [&] {
                if (!result) {
                    if constexpr (std::is_arithmetic_v<Ts>) {
                        Ts temp{};
                        result = deserialize_arithmetic_force_match(temp, j, options);
                        if (result) { t = std::move(temp); }
                    }
                }
            }(),
            ...
        );
    }(std::type_identity<std::remove_cvref_t<T>>{});
    if (!result) {
        [&]<typename... Ts>(std::type_identity<std::variant<Ts...>>) {
            (
                [&] {
                    if (!result) {
                        Ts temp{};
                        result = deserialize_impl(temp, j, options, PriorityTag<10>{});
                        if (result) { t = std::move(temp); }
                    }
                }(),
                ...
            );
        }(std::type_identity<std::remove_cvref_t<T>>{});
    }
    return result;
}

template <concepts::is_string_convertible T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<3>) noexcept {
    if (j.is_string()) {
        t = j.get<std::string>();
        return true;
    }
    return false;
}

template <concepts::is_tuple_like T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<3>) noexcept {
    if (!j.is_array()) { return false; }
    if (j.size() != std::tuple_size_v<T>) { return false; }
    bool result{true};
    std::apply(
        [&](auto&... args) {
            size_t index{0};
            (([&](auto& arg) {
                 result &= deserialize_impl(arg, j[index], options, PriorityTag<10>{});
                 index++;
             }(args)),
             ...);
        },
        t
    );
    return result;
}

template <concepts::is_array_like T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<2>) noexcept {
    if (!j.is_array()) { return false; }
    t.clear();
    bool result{true};
    for (size_t i = 0; i < j.size(); i++) {
        if constexpr (traits::has_emplace_back_method_v<T>) {
            result &= deserialize_impl(t.emplace_back(), j[i], options, PriorityTag<10>{});
        } else {
            typename std::remove_cvref_t<T>::value_type temp{};
            result &= deserialize_impl(temp, j[i], options, PriorityTag<10>{});
            t.insert(std::move(temp));
        }
    }
    return result;
}

template <concepts::is_associative T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<2>) noexcept {
    using KT = typename std::remove_cvref_t<T>::key_type;
    static_assert(traits::is_stringifiable_type_v<KT>, "the key type of the associative container must be convertible to a string");

    if (!j.is_object()) { return false; }
    bool result{true};
    t.clear();
    for (const auto& [k, v] : j.items()) {
        if (auto kt = string_utils::detail::string_to_type<KT>(k)) {
            result &= deserialize_impl(t[*kt], v, options, PriorityTag<10>{});
            continue;
        }
        result &= false;
    }
    return result;
}

template <concepts::is_aggregate T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<1>) noexcept {
    if (j.is_object()) {
        bool result{true};
        boost::pfr::for_each_field_with_name(t, [&](std::string_view name, auto& val) {
            if constexpr (traits::is_renamed_v<decltype(val)>) { name = val.view(); }
            if (j.contains(name)) {
                result &= deserialize_impl(val, j[name], options, PriorityTag<10>{});
                // TODO???
            }
        });
        return result;
    }
    return false;
}

template <typename T>
inline bool deserialize_impl(T&, const JsoncType&, const Options&, PriorityTag<0>) noexcept {
    static_assert(traits::always_false_v<T>, "type is not reflectable.");
}

} // namespace

} // namespace jsonc::reflection
