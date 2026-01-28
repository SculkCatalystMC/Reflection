#pragma once
#include <filesystem>
#include <fstream>
#include <optional>

namespace jsonc::reflection::file_utils {

std::optional<std::string> read_file(std::filesystem::path const& path) noexcept {
    if (!std::filesystem::exists(path)) { return std::nullopt; }
    auto file = std::ifstream(path, std::ios_base::in);
    return std::string(std::istreambuf_iterator<char>(file), {});
}

bool write_file(std::filesystem::path const& path, std::string_view content) noexcept {
    if (!std::filesystem::exists(path.parent_path())) { std::filesystem::create_directories(path.parent_path()); }
    auto file = std::ofstream(path, std::ios_base::out);
    file << content;
    return true;
}

} // namespace jsonc::reflection::file_utils
