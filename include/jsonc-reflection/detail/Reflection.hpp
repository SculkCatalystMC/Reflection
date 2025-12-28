#pragma once
#include "endgate/base/Concepts.hpp"
#include <boost/pfr.hpp>

namespace endgate::reflection {

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
    boost::pfr::for_each_field_with_name(
        std::forward<T>(inst),
        [f = std::forward<F>(func)](std::string_view name, auto& value) {
            if constexpr (concepts::IsRenamed<decltype(value)>) {
                f(value.view(), *value);
            } else {
                f(name, value);
            }
        }
    );
}

} // namespace endgate::reflection
