// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <filesystem>
#include <fstream>
#include <optional>

namespace sculk::reflection::file_utils {

inline std::optional<std::string> read_file(const std::filesystem::path& path) noexcept {
    if (!std::filesystem::exists(path)) { return std::nullopt; }
    auto file = std::ifstream(path, std::ios_base::in);
    return std::string(std::istreambuf_iterator<char>(file), {});
}

inline bool write_file(const std::filesystem::path& path, std::string_view content) noexcept {
    if (!std::filesystem::exists(path.parent_path())) { std::filesystem::create_directories(path.parent_path()); }
    auto file = std::ofstream(path, std::ios_base::out);
    file << content;
    return true;
}

} // namespace sculk::reflection::file_utils
