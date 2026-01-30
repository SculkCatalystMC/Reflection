#pragma once
#include "jsonc/detail/reflection/annotated.hpp"
#include "jsonc/detail/reflection/deserialization.hpp"
#include "jsonc/detail/reflection/dispatcher.hpp"
#include "jsonc/detail/reflection/file_utils.hpp"
#include "jsonc/detail/reflection/fixed_number.hpp"
#include "jsonc/detail/reflection/fixed_string.hpp"
#include "jsonc/detail/reflection/renamed.hpp"
#include "jsonc/detail/reflection/serialization.hpp"
#include <chrono>

namespace jsonc::reflection {

template <typename T, concepts::is_key_formatter F>
bool load_file(T& t, const std::filesystem::path& path, const F& key_formatter, const Options& options = {}) noexcept {
    JsoncType data{};

    std::optional<std::string> content = file_utils::read_file(path);
    if (content) {
        if (auto value = parse(*content, options.allow_trailing_comma, options.ignore_comments)) { data = *value; }
    }

    bool result = deserialize(t, data, key_formatter, options);

    if (options.rewrite_policy == RewritePolicy::Always || (options.rewrite_policy == RewritePolicy::OnError && !result)
        || options.rewrite_policy == RewritePolicy::OnFormat) {
        JsoncType res = serialize(t, key_formatter, options);
        if (options.keep_extra_comments && !data.is_null() && !options.ignore_comments) {
            data.move_comments_to_before();
            res.merge_comments(data);
        }

        if (options.back_up_file_on_error && content && !result) {
            std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm     local_tm;
#ifdef _WIN32
            localtime_s(&local_tm, &time);
#else
            localtime_r(&time, &local_tm);
#endif
            std::ostringstream oss;
            oss << std::put_time(&local_tm, options.back_up_time_suffix.c_str());
            auto filename  = path.stem();
            filename      += oss.str();
            filename      += path.extension();
            if (!options.back_up_file_folder.empty() && std::filesystem::is_directory(options.back_up_file_folder)) {
                file_utils::write_file(std::filesystem::absolute(options.back_up_file_folder).parent_path() / filename, *content);
            } else {
                file_utils::write_file(std::filesystem::absolute(path).parent_path() / filename, *content);
            }
        }

        auto file = res.dump(options.indent, options.ensure_ascii, options.ignore_comments, options.multi_line_comments_format);
        if (options.rewrite_policy == RewritePolicy::OnFormat && content) {
            if (file != *content) { file_utils::write_file(path, file); }
        } else {
            file_utils::write_file(path, file);
        }
        if (!content) { result = true; }
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
    return file_utils::write_file(path, res.dump(options.indent, options.ensure_ascii, options.ignore_comments, options.multi_line_comments_format));
}

template <typename T>
bool save_file(const T& t, const std::filesystem::path& path, const Options& options = {}) noexcept {
    return save_file(t, path, builtin_key_formatter::default_key_formatter, options);
}

} // namespace jsonc::reflection