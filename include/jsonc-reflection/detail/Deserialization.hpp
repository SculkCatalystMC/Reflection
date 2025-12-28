#pragma once
#include "endgate/base/Concepts.hpp"
#include "endgate/base/Expected.hpp"
#include "endgate/base/Meta.hpp"
#include "endgate/reflection/Reflection.hpp"
#include <magic_enum/magic_enum.hpp>

namespace endgate::reflection {

namespace detail {
template <concepts::IsAnnotated T, typename J>
inline Expected<> deserialize_impl(T& opt, J&& j, meta::PriorityTag<6>);
template <concepts::IsDispatcher T, typename J>
inline Expected<> deserialize_impl(T& d, J&& j, meta::PriorityTag<5>);
template <concepts::IsOptional T, typename J>
inline Expected<> deserialize_impl(T& opt, J&& j, meta::PriorityTag<5>);
template <concepts::IsString T, typename J>
inline Expected<> deserialize_impl(T& str, J&& j, meta::PriorityTag<4>);
template <concepts::IsRanged T, typename J>
inline Expected<> deserialize_impl(T& str, J&& j, meta::PriorityTag<4>);
template <concepts::IsTupleLike T, typename J>
inline Expected<> deserialize_impl(T& tuple, J&& j, meta::PriorityTag<3>);
template <concepts::IsVariant T, typename J>
inline Expected<> deserialize_impl(T& tuple, J&& j, meta::PriorityTag<3>);
template <concepts::IsArrayLike T, typename J>
inline Expected<> deserialize_impl(T& arr, J&& j, meta::PriorityTag<2>);
template <concepts::IsAssociative T, typename J>
inline Expected<> deserialize_impl(T& map, J const& j, meta::PriorityTag<2>);
template <concepts::IsReflectable T, typename J>
inline Expected<> deserialize_impl(T& obj, J const& j, meta::PriorityTag<1>);
template <traits::Require<std::is_enum> T, typename J>
inline Expected<> deserialize_impl(T& e, J const& j, meta::PriorityTag<1>);
template <typename T, typename J>
inline Expected<> deserialize_impl(T& obj, J const& j, meta::PriorityTag<0>)
    requires(std::convertible_to<J, T>);
} // namespace detail

template <typename T, typename J>
[[nodiscard]] inline Expected<> deserialize(T& t, J&& j) try {
    return detail::deserialize_impl<T>(t, std::forward<J>(j), meta::PriorityTag<6>{});
} catch (...) {
    return makeExceptionError();
}

namespace detail {
template <typename T, typename J>
[[nodiscard]] inline Expected<T> deserialize_to(J&& j) noexcept {
    Expected<T> res{};
    if (auto d = deserialize<T>(*res, std::forward<J>(j)); !d) {
        res = forwardError(d.error());
    }
    return res;
}
template <concepts::IsAnnotated T, typename J>
inline Expected<> deserialize_impl(T& obj, J&& j, meta::PriorityTag<6>) {
    using ValueType = std::remove_cvref_t<decltype(*obj)>;
    return deserialize<ValueType>(*obj, std::forward<J>(j));
}
template <concepts::IsDispatcher T, typename J>
inline Expected<> deserialize_impl(T& d, J&& j, meta::PriorityTag<5>) {
    return deserialize<typename T::storage_type>(*d, std::forward<J>(j));
}
template <concepts::IsOptional T, typename J>
inline Expected<> deserialize_impl(T& opt, J&& j, meta::PriorityTag<5>) {
    Expected<> res;
    if (j.is_null()) {
        opt = std::nullopt;
    } else {
        if (!opt) opt.emplace();
        res = deserialize<typename T::value_type>(*opt, std::forward<J>(j));
    }
    return res;
}
template <concepts::IsString T, typename J>
inline Expected<> deserialize_impl(T& str, J&& j, meta::PriorityTag<4>) {
    if (!j.is_string()) {
        return makeStringError("field must be a string");
    }
    str = std::string(std::forward<J>(j));
    return {};
}
template <concepts::IsRanged T, typename J>
inline Expected<> deserialize_impl(T& num, J&& j, meta::PriorityTag<4>) {
    if (!j.is_number()) {
        return makeStringError("field must be a number");
    }
    using ValueType = std::remove_cvref_t<decltype(*num)>;
    return deserialize<ValueType>(*num, std::forward<J>(j));
}
template <concepts::IsTupleLike T, typename J>
inline Expected<> deserialize_impl(T& tuple, J&& j, meta::PriorityTag<3>) {
    if (!j.is_array()) return makeStringError("field must be an array");
    if (j.size() != std::tuple_size_v<T>)
        return makeStringError(fmt::format("array size must be {}", std::tuple_size_v<T>));
    Expected<> res{};
    std::apply(
        [&](auto&... args) {
            size_t iter{0};
            (([&](auto& arg) {
                 if (res) {
                     res = deserialize<std::remove_cvref_t<decltype(arg)>>(arg, static_cast<J&&>(j[iter]));
                     if (!res) res = makeStringError("Failed to deserialize tuple {0}: {1}", iter, res.error());
                     iter++;
                 }
             }(args)),
             ...);
        },
        tuple
    );
    return res;
}
template <concepts::IsVariant T, typename J>
inline Expected<> deserialize_impl(T& variant, J&& j, meta::PriorityTag<3>) {
    using VariantType          = std::remove_cvref_t<T>;
    bool       success         = false;
    const auto try_deserialize = [&]<typename AltType>(std::type_identity<AltType>) {
        if (!success) {
            AltType data;
            if (deserialize<AltType>(data, std::forward<J>(j))) {
                variant = std::move(data);
                success = true;
            }
        }
    };
    [&]<typename... Ts>(std::variant<Ts...>*) {
        (try_deserialize(std::type_identity<Ts>{}), ...);
    }(static_cast<VariantType*>(nullptr));
    return success ? Expected<void>{} : makeStringError("field does not match any variant type");
}
template <concepts::IsArrayLike T, typename J>
inline Expected<> deserialize_impl(T& arr, J&& j, meta::PriorityTag<2>) {
    if (!j.is_array()) {
        return makeStringError("field must be an array");
    }
    using ArrayType = std::remove_cvref_t<T>;
    using ValueType = typename ArrayType::value_type;
    if constexpr (requires(T a) { a.clear(); }) {
        arr.clear();
    }
    if constexpr (requires(T a, ValueType v) { a.push_back(v); }) {
        for (size_t i = 0; i < j.size(); i++) {
            if (auto res = deserialize<ValueType>(arr.emplace_back(), static_cast<J&&>(j[i])); !res) {
                res = makeStringError("Failed to deserialize array {0}: {1}", i, res.error());
                return res;
            }
        }
    } else if constexpr (requires(T a, ValueType v) { a.insert(v); }) {
        for (size_t i = 0; i < j.size(); i++) {
            ValueType tmp{};
            if (auto res = deserialize<ValueType>(tmp, static_cast<J&&>(j[i])); !res) {
                res = makeStringError("Failed to deserialize array {0}: {1}", i, res.error());
                return res;
            }
            arr.insert(std::move(tmp));
        }
    }
    return {};
}
template <concepts::IsAssociative T, typename J>
inline Expected<> deserialize_impl(T& map, J const& j, meta::PriorityTag<2>) {
    using ObjectType = std::remove_cvref_t<T>;
    static_assert(
        (concepts::IsString<typename ObjectType::key_type> || std::is_enum_v<typename ObjectType::key_type>),
        "the key type of the associative container must be convertible to a string"
    );
    if (!j.is_object()) return makeStringError("field must be an object");
    map.clear();
    for (auto&& [k, v] : j.items()) {
        if constexpr (concepts::IsString<typename ObjectType::key_type>) {
            if (auto res = deserialize<typename ObjectType::mapped_type>(
                    map[static_cast<ObjectType::key_type>(k)],
                    std::forward<decltype(v)>(v)
                );
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
template <concepts::IsReflectable T, typename J>
inline Expected<> deserialize_impl(T& obj, J const& j, meta::PriorityTag<1>) {
    Expected<> res;
    if (!j.is_object()) {
        res = makeStringError("field must be an object");
        return res;
    }
    forEachFieldWithName(obj, [&](std::string_view name, auto& member) {
        using MemberType = std::remove_cvref_t<decltype((member))>;
        if (j.contains(name)) {
            if constexpr (requires(MemberType& o, J const& s) { deserialize<MemberType>(o, s); }) {
                auto status = deserialize<MemberType>(member, j[name]);
                if (!status) res = makeStringError("Failed to deserialize field {0}: {1}", name, status.error());
            } else {
                static_assert(traits::always_false<MemberType>, "this type can't deserialize");
            }
        }
    });
    return res;
}
template <traits::Require<std::is_enum> T, typename J>
inline Expected<> deserialize_impl(T& e, J const& j, meta::PriorityTag<1>) {
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
template <typename T, typename J>
inline Expected<> deserialize_impl(T& obj, J const& j, meta::PriorityTag<0>)
    requires(std::convertible_to<J, T>)
{
    obj = j;
    return {};
}
} // namespace detail

} // namespace endgate::reflection