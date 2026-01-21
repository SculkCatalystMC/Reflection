#pragma once
#include "jsonc-reflection/detail/Concepts.hpp"
#include "jsonc-reflection/detail/Expected.hpp"
#include "jsonc-reflection/detail/Meta.hpp"
#include "jsonc-reflection/detail/Reflection.hpp"
#include <magic_enum/magic_enum.hpp>

#ifndef JSONC_NO_EXCEPTION
#define JSONC_NO_EXCEPTION
#endif
#include <jsonc/jsonc.hpp>

namespace jsonc_reflection {

namespace detail {

template <concepts::IsAnnotated T>
inline Expected<> deserialize_impl(T& opt, const jsonc::JsoncType& j, PriorityTag<6>);

template <concepts::IsDispatcher T>
inline Expected<> deserialize_impl(T& d, const jsonc::JsoncType& j, PriorityTag<5>);

template <concepts::IsOptional T>
inline Expected<> deserialize_impl(T& opt, const jsonc::JsoncType& j, PriorityTag<5>);

template <concepts::IsString T>
inline Expected<> deserialize_impl(T& str, const jsonc::JsoncType& j, PriorityTag<4>);

template <concepts::IsRanged T>
inline Expected<> deserialize_impl(T& str, const jsonc::JsoncType& j, PriorityTag<4>);

template <concepts::IsTupleLike T>
inline Expected<> deserialize_impl(T& tuple, const jsonc::JsoncType& j, PriorityTag<3>);

template <concepts::IsVariant T>
inline Expected<> deserialize_impl(T& tuple, const jsonc::JsoncType& j, PriorityTag<3>);

template <concepts::IsArrayLike T>
inline Expected<> deserialize_impl(T& arr, const jsonc::JsoncType& j, PriorityTag<2>);

template <concepts::IsAssociative T>
inline Expected<> deserialize_impl(T& map, const jsonc::JsoncType& j, PriorityTag<2>);

template <concepts::IsReflectable T>
inline Expected<> deserialize_impl(T& obj, const jsonc::JsoncType& j, PriorityTag<1>);

template <traits::Require<std::is_enum> T>
inline Expected<> deserialize_impl(T& e, const jsonc::JsoncType& j, PriorityTag<1>);

template <typename T>
    requires(std::convertible_to<jsonc::JsoncType, T>)
inline Expected<> deserialize_impl(T& obj, const jsonc::JsoncType& j, PriorityTag<0>);

} // namespace detail

template <typename T>
[[nodiscard]] inline Expected<> deserialize(T& t, const jsonc::JsoncType& j) noexcept {
    return detail::deserialize_impl<T>(t, j, PriorityTag<6>{});
}

namespace detail {

template <typename T>
[[nodiscard]] inline Expected<T> deserialize_to(const jsonc::JsoncType& j) noexcept {
    Expected<T> res{};
    if (auto d = deserialize<T>(*res, j); !d) { res = forwardError(d.error()); }
    return res;
}

template <concepts::IsAnnotated T>
inline Expected<> deserialize_impl(T& obj, const jsonc::JsoncType& j, PriorityTag<6>) {
    using ValueType = std::remove_cvref_t<decltype(*obj)>;
    return deserialize<ValueType>(*obj, j);
}

template <concepts::IsDispatcher T>
inline Expected<> deserialize_impl(T& d, const jsonc::JsoncType& j, PriorityTag<5>) {
    return deserialize<typename T::storage_type>(*d, j);
}

template <concepts::IsOptional T>
inline Expected<> deserialize_impl(T& opt, const jsonc::JsoncType& j, PriorityTag<5>) {
    Expected<> res;
    if (j.is_null()) {
        opt = std::nullopt;
    } else {
        if (!opt) { opt.emplace(); }
        res = deserialize<typename T::value_type>(*opt, j);
    }
    return res;
}

template <concepts::IsString T>
inline Expected<> deserialize_impl(T& str, const jsonc::JsoncType& j, PriorityTag<4>) {
    if (!j.is_string()) { return makeStringError("field must be a string"); }
    str = j.as<std::string>();
    return {};
}

template <concepts::IsRanged T>
inline Expected<> deserialize_impl(T& num, const jsonc::JsoncType& j, PriorityTag<4>) {
    if (!j.is_number()) { return makeStringError("field must be a number"); }
    using ValueType = std::remove_cvref_t<decltype(*num)>;
    return deserialize<ValueType>(*num, j);
}

template <concepts::IsTupleLike T>
inline Expected<> deserialize_impl(T& tuple, const jsonc::JsoncType& j, PriorityTag<3>) {
    if (!j.is_array()) return makeStringError("field must be an array");
    if (j.size() != std::tuple_size_v<T>) { return makeStringError("array size must be {}", std::tuple_size_v<T>); }
    Expected<> res{};
    std::apply(
        [&](auto&... args) {
            size_t iter{0};
            (([&](auto& arg) {
                 if (res) {
                     res = deserialize<std::remove_cvref_t<decltype(arg)>>(arg, static_cast<const jsonc::JsoncType&>(j[iter]));
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
inline Expected<> deserialize_impl(T& variant, const jsonc::JsoncType& j, PriorityTag<3>) {
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
inline Expected<> deserialize_impl(T& arr, const jsonc::JsoncType& j, PriorityTag<2>) {
    if (!j.is_array()) { return makeStringError("field must be an array"); }
    using ArrayType = std::remove_cvref_t<T>;
    using ValueType = typename ArrayType::value_type;
    if constexpr (requires(T a) { a.clear(); }) { arr.clear(); }
    if constexpr (requires(T a, ValueType v) { a.push_back(v); }) {
        for (size_t i = 0; i < j.size(); i++) {
            if (auto res = deserialize<ValueType>(arr.emplace_back(), static_cast<const jsonc::JsoncType&>(j[i])); !res) {
                res = makeStringError("Failed to deserialize array {0}: {1}", i, res.error());
                return res;
            }
        }
    } else if constexpr (requires(T a, ValueType v) { a.insert(v); }) {
        for (size_t i = 0; i < j.size(); i++) {
            ValueType tmp{};
            if (auto res = deserialize<ValueType>(tmp, static_cast<const jsonc::JsoncType&>(j[i])); !res) {
                res = makeStringError("Failed to deserialize array {0}: {1}", i, res.error());
                return res;
            }
            arr.insert(std::move(tmp));
        }
    }
    return {};
}

template <concepts::IsAssociative T>
inline Expected<> deserialize_impl(T& map, const jsonc::JsoncType& j, PriorityTag<2>) {
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

template <concepts::IsReflectable T>
inline Expected<> deserialize_impl(T& obj, const jsonc::JsoncType& j, PriorityTag<1>) {
    Expected<> res;
    if (!j.is_object()) {
        res = makeStringError("field must be an object");
        return res;
    }
    forEachFieldWithName(obj, [&](std::string_view name, auto& member) {
        using MemberType = std::remove_cvref_t<decltype(member)>;
        if (j.contains(name)) {
            if constexpr (requires(MemberType& o, const jsonc::JsoncType& s) { deserialize<MemberType>(o, s); }) {
                auto status = deserialize<MemberType>(member, j[name]);
                if (!status) res = makeStringError("Failed to deserialize field {0}: {1}", name, status.error());
            } else {
                static_assert(traits::always_false<MemberType>, "this type can't deserialize");
            }
        }
    });
    return res;
}

template <traits::Require<std::is_enum> T>
inline Expected<> deserialize_impl(T& e, const jsonc::JsoncType& j, PriorityTag<1>) {
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
    requires(std::convertible_to<jsonc::JsoncType, T>)
inline Expected<> deserialize_impl(T& obj, const jsonc::JsoncType& j, PriorityTag<0>) {
    obj = j;
    return {};
}

} // namespace detail

} // namespace jsonc_reflection