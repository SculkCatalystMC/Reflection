// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <boost/pfr.hpp>

namespace sculk::reflection::pfr {

template <typename T, typename F>
constexpr void for_each_field_with_name(T&& value, F&& func) {
    return boost::pfr::for_each_field(std::forward<T>(value), [f = std::forward<F>(func)](auto&& field, auto index) mutable {
        using IndexType     = decltype(index);
        using FieldType     = decltype(field);
        constexpr auto name = boost::pfr::detail::get_name<std::remove_reference_t<T>, IndexType::value>();
        if constexpr (std::is_invocable_v<F, std::string_view, FieldType, IndexType>) {
            f(name, std::forward<FieldType>(field), index);
        } else {
            f(name, std::forward<FieldType>(field));
        }
    });
}

} // namespace sculk::reflection::pfr