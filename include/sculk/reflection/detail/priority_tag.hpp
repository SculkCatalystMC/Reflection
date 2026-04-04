// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

namespace sculk::reflection {

template <size_t N>
struct priority_tag : priority_tag<N - 1> {};
template <>
struct priority_tag<0> {};

} // namespace sculk::reflection