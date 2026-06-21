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
#include <string>
#include <string_view>

namespace sculk::reflection::file_utils {

inline std::optional<std::string> read_file(const std::filesystem::path& path) {
    std::error_code ec{};
    if (!std::filesystem::exists(path, ec) || ec) { return std::nullopt; }

    auto file = std::ifstream(path, std::ios_base::in | std::ios_base::binary);
    if (!file.is_open()) { return std::nullopt; }

    return std::string(std::istreambuf_iterator<char>(file), {});
}

inline bool write_file(const std::filesystem::path& path, std::string_view content) {
    std::error_code ec{};
    const auto      parent = path.parent_path();
    if (!parent.empty() && !std::filesystem::exists(parent, ec)) {
        if (ec || !std::filesystem::create_directories(parent, ec) || ec) { return false; }
    } else if (ec) {
        return false;
    }

    auto file = std::ofstream(path, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (!file.is_open()) { return false; }

    file << content;
    return static_cast<bool>(file);
}

} // namespace sculk::reflection::file_utils
