#pragma once
#include "jsonc/detail/reflection/concepts.hpp"
#include "jsonc/detail/reflection/jsonc_header.hpp"
#include "jsonc/detail/reflection/options.hpp"
#include "jsonc/detail/reflection/priority_tag.hpp"
#include <boost/pfr.hpp>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

namespace jsonc::reflection {

namespace detail {

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

} // namespace detail

template <typename T>
[[nodiscard]] inline bool deserialize(T& t, const JsoncType& j, const Options& options = {}) noexcept
    requires(
        traits::is_reflectable_v<std::remove_cvref_t<decltype(t)>>
        || (traits::is_reflectable_v<std::remove_cvref_t<decltype(*t)>> && traits::is_annotated_v<std::remove_cvref_t<decltype(t)>>)
    )
{
    if constexpr (traits::is_annotated_v<std::remove_cvref_t<decltype(t)>>) {
        bool result = detail::deserialize_impl(*t, j, options, PriorityTag<10>{});
        t.set_comments(j.get_before_comments());
        return result;
    } else {
        return detail::deserialize_impl(t, j, options, PriorityTag<10>{});
    }
}

namespace detail {

template <concepts::is_arithmetic T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<10>) noexcept {
    using RT = std::remove_cvref_t<T>;
    switch (j.type()) {
    case ValueType::Boolean:
        t = static_cast<RT>(j.get<bool>());
    case ValueType::Signed:
        t = static_cast<RT>(j.get<int64_t>());
    case ValueType::Unsigned:
        t = static_cast<RT>(j.get<uint64_t>());
    case ValueType::Float: {
        t = static_cast<RT>(j.get<double>());
        return true;
    }
    default:
        return false;
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
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<5>) noexcept {
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

template <concepts::is_variant T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<4>) noexcept {
    // TODO
    return false;
}

template <concepts::is_string_convertible T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options&, PriorityTag<3>) noexcept {
    if (j.is_string()) { t = j.get<std::string>(); }
    return false;
}

template <concepts::is_tuple_like T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<3>) noexcept {
    return false;
}

template <concepts::is_array_like T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<2>) noexcept {
    return false;
}

template <concepts::is_associative T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<2>) noexcept {
    return false;
}

template <concepts::is_aggregate T>
inline bool deserialize_impl(T& t, const JsoncType& j, const Options& options, PriorityTag<1>) noexcept {
    bool result = true;
    boost::pfr::for_each_field_with_name(t, [&](std::string_view name, auto& val) {
        if constexpr (traits::is_renamed_v<decltype(val)>) { name = val.view(); }
        if (j.contains(name)) {
            result &= deserialize_impl(val, j[name], options, PriorityTag<10>{});
            // TODO???
        }
    });
    return result;
}

template <typename T>
inline bool deserialize_impl(T&, const JsoncType&, const Options&, PriorityTag<0>) noexcept {
    static_assert(traits::always_false_v<T>, "type is not reflectable.");
}

} // namespace detail

} // namespace jsonc::reflection

/*

template <concepts::IsTupleLike T>
inline Expected<> deserialize_impl(T& tuple, const JsoncType& j, PriorityTag<3>) {
    if (!j.is_array()) return makeStringError("field must be an array");
    if (j.size() != std::tuple_size_v<T>) { return makeStringError("array size must be {}", std::tuple_size_v<T>); }
    Expected<> res{};
    std::apply(
        [&](auto&... args) {
            size_t iter{0};
            (([&](auto& arg) {
                 if (res) {
                     res = deserialize<std::remove_cvref_t<decltype(arg)>>(arg, static_cast<const JsoncType&>(j[iter]));
                     if (!res) { res = makeStringError("Failed to deserialize tuple {0}: {1}", iter, res.error()); }
                     iter++;
                 }
             }(args)),
             ...);
        },
        tuple
    );
    return res;
}

template <concepts::IsVariant T>
inline Expected<> deserialize_impl(T& variant, const JsoncType& j, PriorityTag<3>) {
    using VariantType          = std::remove_cvref_t<T>;
    bool       success         = false;
    const auto try_deserialize = [&]<typename AltType>(std::type_identity<AltType>) {
        if (!success) {
            AltType data;
            if (deserialize<AltType>(data, j)) {
                variant = std::move(data);
                success = true;
            }
        }
    };
    [&]<typename... Ts>(std::variant<Ts...>*) { (try_deserialize(std::type_identity<Ts>{}), ...); }(static_cast<VariantType*>(nullptr));
    return success ? Expected<void>{} : makeStringError("field does not match any variant type");
}

template <concepts::IsArrayLike T>
inline Expected<> deserialize_impl(T& arr, const JsoncType& j, PriorityTag<2>) {
    if (!j.is_array()) { return makeStringError("field must be an array"); }
    using ArrayType = std::remove_cvref_t<T>;
    using ValueType = typename ArrayType::value_type;
    if constexpr (requires(T a) { a.clear(); }) { arr.clear(); }
    if constexpr (requires(T a, ValueType v) { a.push_back(v); }) {
        for (size_t i = 0; i < j.size(); i++) {
            if (auto res = deserialize<ValueType>(arr.emplace_back(), static_cast<const JsoncType&>(j[i])); !res) {
                res = makeStringError("Failed to deserialize array {0}: {1}", i, res.error());
                return res;
            }
        }
    } else if constexpr (requires(T a, ValueType v) { a.insert(v); }) {
        for (size_t i = 0; i < j.size(); i++) {
            ValueType tmp{};
            if (auto res = deserialize<ValueType>(tmp, static_cast<const JsoncType&>(j[i])); !res) {
                res = makeStringError("Failed to deserialize array {0}: {1}", i, res.error());
                return res;
            }
            arr.insert(std::move(tmp));
        }
    }
    return {};
}

template <concepts::IsAssociative T>
inline Expected<> deserialize_impl(T& map, const JsoncType& j, PriorityTag<2>) {
    using ObjectType = std::remove_cvref_t<T>;
    static_assert(
        (concepts::IsString<typename ObjectType::key_type> || std::is_enum_v<typename ObjectType::key_type>),
        "the key type of the associative container must be convertible to a string"
    );
    if (!j.is_object()) return makeStringError("field must be an object");
    map.clear();
    for (auto&& [k, v] : j.items()) {
        if constexpr (concepts::IsString<typename ObjectType::key_type>) {
            if (auto res = deserialize<typename ObjectType::mapped_type>(map[static_cast<ObjectType::key_type>(k)], std::forward<decltype(v)>(v));
                !res) {
                res = makeStringError("Failed to deserialize object {0}: {1}", k, res.error());
                return res;
            }
        } else {
            if (auto res = deserialize<typename ObjectType::mapped_type>(
                    map[magic_enum::enum_cast<typename ObjectType::key_type>(k).value()],
                    std::forward<decltype(v)>(v)
                );
                !res) {
                res = makeStringError("Failed to deserialize object {0}: {1}", k, res.error());
                return res;
            }
        }
    }
    return {};
}

template <traits::Require<std::is_enum> T>
inline Expected<> deserialize_impl(T& e, const JsoncType& j, PriorityTag<1>) {
    using enum_type = std::remove_cvref_t<T>;
    if (j.is_string()) {
        if constexpr (magic_enum::detail::subtype_v<enum_type> == magic_enum::detail::enum_subtype::flags) {
            e = magic_enum::enum_cast<enum_type>(std::string{j}).value();
        }
    } else {
        e = (enum_type)(std::underlying_type_t<enum_type>{j});
    }
    return {};
}

template <typename T>
    requires(std::convertible_to<JsoncType, T>)
inline Expected<> deserialize_impl(T& obj, const JsoncType& j, PriorityTag<0>) {
    obj = j;
    return {};
}

} // namespace detail
*/