#pragma once
#include "jsonc/detail/reflection/annotated.hpp"
#include "jsonc/detail/reflection/deserialization.hpp"
#include "jsonc/detail/reflection/dispatcher.hpp"
#include "jsonc/detail/reflection/file_utils.hpp"
#include "jsonc/detail/reflection/fixed_number.hpp"
#include "jsonc/detail/reflection/fixed_string.hpp"
#include "jsonc/detail/reflection/renamed.hpp"
#include "jsonc/detail/reflection/serialization.hpp"

namespace jsonc::reflection {

template <typename T, concepts::is_key_formatter F>
bool load_file(T& t, const std::filesystem::path& path, const F& key_formatter, const Options& options = {}) noexcept {
    bool                     result{false};
    std::optional<JsoncType> data{};

    if (auto content = file_utils::read_file(path)) {
        data = parse(*content, options.allow_trailing_comma, options.ignore_comments);
        if (data) { result = deserialize(t, *data, key_formatter, options); }
    }

    if (options.always_overwrite || (options.overwrite_when_error && !result)) {
        JsoncType res = serialize(t, key_formatter, options);
        if (options.keep_extra_comments && data && !options.ignore_comments) {
            data->move_comments_to_before();
            res.merge_comments(*data);
        }
        file_utils::write_file(path, res.dump(options.indent, options.ensure_ascii, options.ignore_comments));
    }

    return result;
}

template <typename T>
bool load_file(T& t, const std::filesystem::path& path, const Options& options = {}) noexcept {
    return load_file(t, path, builtin_key_formatter::default_key_formatter, options);
}

template <typename T, concepts::is_key_formatter F>
bool save_file(const T& t, const std::filesystem::path& path, const F& key_formatter, const Options& options = {}) noexcept {
    JsoncType res = serialize(t, key_formatter, options);
    if (options.keep_extra_comments && !options.ignore_comments) {
        if (auto content = file_utils::read_file(path)) {
            if (auto data = parse(*content, options.allow_trailing_comma, options.ignore_comments)) {
                data->move_comments_to_before();
                res.merge_comments(*data);
            };
        }
    }
    return file_utils::write_file(path, res.dump(options.indent, options.ensure_ascii, options.ignore_comments));
}

template <typename T>
bool save_file(const T& t, const std::filesystem::path& path, const Options& options = {}) noexcept {
    return save_file(t, path, builtin_key_formatter::default_key_formatter, options);
}

} // namespace jsonc::reflection