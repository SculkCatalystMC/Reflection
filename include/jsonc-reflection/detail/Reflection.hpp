#pragma once
#include "jsonc-reflection/detail/Concepts.hpp"
#include <boost/pfr.hpp>

namespace jsonc_reflection {

template <class T>
constexpr auto const member_name_array_v = boost::pfr::names_as_array<std::remove_cvref_t<T>>();

template <concepts::IsReflectable T, typename F>
constexpr void forEachField(T&& inst, F&& func) {
    boost::pfr::for_each_field(std::forward<T>(inst), [f = std::forward<F>(func)](auto& value) {
        if constexpr (concepts::IsRenamed<decltype(value)>) {
            f(*value);
        } else {
            f(value);
        }
    });
}

template <concepts::IsReflectable T, typename F>
constexpr void forEachFieldWithName(T&& inst, F&& func) {
    // boost::pfr::for_each_field_with_name(std::forward<T>(inst), [f = std::forward<F>(func)](std::string_view name, auto& value) {
    //     if constexpr (concepts::IsRenamed<decltype(value)>) {
    //         f(value.view(), *value);
    //     } else {
    //         f(name, value);
    //     }
    // });
    boost::pfr::for_each_field(std::forward<T>(inst), [func = std::forward<F>(func)](auto&& field, std::size_t idx) {
        if constexpr (concepts::IsRenamed<decltype(field)>) {
            func(field.view(), *field);
        } else {
            func(member_name_array_v<T>[idx], field);
        }
    });
}

} // namespace jsonc_reflection