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

template <typename T>
bool load_config(T& t, const std::filesystem::path& path, const Options& options = {}) noexcept
    requires(
        traits::is_reflectable_v<std::remove_cvref_t<decltype(t)>>
        || (traits::is_reflectable_v<std::remove_cvref_t<decltype(*t)>> && traits::is_annotated_v<std::remove_cvref_t<decltype(t)>>)
    )
{
    bool                     result{false};
    std::optional<JsoncType> data{};

    if (auto content = file_utils::read_file(path)) {
        data = parse(*content, options.allow_trailing_comma, options.ignore_comments);
        if (data) { result = deserialize(t, *data, options); }
    }

    if (options.always_overwrite || (options.overwrite_when_error && !result)) {
        JsoncType res = serialize(t, options);
        if (options.keep_extra_comments && data && !options.ignore_comments) {
            data->move_comments_to_before();
            res.merge_comments(*data);
        }
        file_utils::write_file(path, res.dump(options.indent, options.ensure_ascii, options.ignore_comments));
    }

    return result;
}

} // namespace jsonc::reflection