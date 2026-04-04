#pragma once
#include "sculk/reflection/annotated.hpp"
#include "sculk/reflection/dispatcher.hpp"
#include "sculk/reflection/file_utils.hpp"
#include "sculk/reflection/fixed_number.hpp"
#include "sculk/reflection/fixed_string.hpp"
#include "sculk/reflection/jsonc/detail/deserialization.hpp"
#include "sculk/reflection/jsonc/detail/serialization.hpp"
#include "sculk/reflection/renamed.hpp"
#include <chrono>

namespace sculk::reflection::jsonc {

template <bool IsOrdered = true, bool AllowComments = true, typename T, concepts::is_key_formatter F>
bool load_file(T& t, const std::filesystem::path& path, const F& key_formatter, const options& options = {}) noexcept {
    detail::basic_jsonc<IsOrdered, AllowComments> data{};

    bool result{false};

    std::optional<std::string> content = file_utils::read_file(path);
    if (content) {
        if (auto value = detail::basic_jsonc<IsOrdered, AllowComments>::parse(
                *content,
                options.allow_trailing_comma,
                options.ignore_comments,
                options.float_keep_precision
            )) {
            data   = *value;
            result = deserialize<IsOrdered, AllowComments>(t, data, key_formatter, options);
        }
    }

    if (options.rewrite_policy == rewrite_policy::always || (options.rewrite_policy == rewrite_policy::error && !result)
        || options.rewrite_policy == rewrite_policy::format) {
        detail::basic_jsonc<IsOrdered, AllowComments> res = serialize<IsOrdered, AllowComments>(t, key_formatter, options);
        if constexpr (AllowComments) {
            if (options.keep_extra_comments && !data.is_null() && !options.ignore_comments) {
                data.move_comments_to_before();
                res.merge_comments(data);
            }
        }

        if (options.back_up_file_on_error && content && !content->empty() && !result) {
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
        if (options.rewrite_policy == rewrite_policy::format && content) {
            if (file != *content) { file_utils::write_file(path, file); }
        } else {
            file_utils::write_file(path, file);
        }
        if (!content) { result = true; }
    }

    return result;
}

template <bool IsOrdered = true, bool AllowComments = true, typename T>
bool load_file(T& t, const std::filesystem::path& path, const options& options = {}) noexcept {
    return load_file<IsOrdered, AllowComments>(t, path, builtin_key_formatter::default_key_formatter, options);
}

template <bool IsOrdered = true, bool AllowComments = true, typename T, concepts::is_key_formatter F>
bool save_file(const T& t, const std::filesystem::path& path, const F& key_formatter, const options& options = {}) noexcept {
    detail::basic_jsonc<IsOrdered, AllowComments> res = serialize<IsOrdered, AllowComments>(t, key_formatter, options);
    if (options.keep_extra_comments && !options.ignore_comments) {
        if (auto content = file_utils::read_file(path)) {
            if (auto data = detail::basic_jsonc<IsOrdered, AllowComments>::parse(
                    *content,
                    options.allow_trailing_comma,
                    options.ignore_comments,
                    options.float_keep_precision
                )) {
                data->move_comments_to_before();
                res.merge_comments(*data);
            };
        }
    }
    return file_utils::write_file(path, res.dump(options.indent, options.ensure_ascii, options.ignore_comments, options.multi_line_comments_format));
}

template <bool IsOrdered = true, bool AllowComments = true, typename T>
bool save_file(const T& t, const std::filesystem::path& path, const options& options = {}) noexcept {
    return save_file<IsOrdered, AllowComments>(t, path, builtin_key_formatter::default_key_formatter, options);
}

} // namespace sculk::reflection::jsonc