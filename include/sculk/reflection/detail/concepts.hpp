// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

namespace sculk::reflection::concepts {

template <typename T>
concept is_enum = std::is_enum_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_arithmetic = std::is_arithmetic_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_number = std::is_arithmetic_v<std::remove_cvref_t<T>> && !std::same_as<std::remove_cvref_t<T>, bool>;

template <typename T, typename Arg0>
concept has_call_func_with_arg = requires(T l, Arg0 a) {
    { l.call(a) } noexcept;
};

template <typename T>
concept has_call_func_no_arg = requires(T l) {
    { l.call() } noexcept;
};

template <typename T, typename Arg0>
concept is_dispatcher_listener =
    std::default_initializable<T> && std::is_nothrow_default_constructible_v<T> && (has_call_func_with_arg<T, Arg0> || has_call_func_no_arg<T>);

} // namespace sculk::reflection::concepts
