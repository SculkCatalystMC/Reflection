# Reflection

[中文文档](README.zh.md)

> 🧩 C++23 Header-only Reflection for JSON and JSONC

A C++23 header-only reflection library focused on JSON and JSON with Comments (JSONC) mapping.

It is designed for config-centric workflows where a C++ struct is the single source of truth:

- Define config with regular C++ types.
- Deserialize from JSON/JSONC into typed objects.
- Serialize typed objects back to JSON/JSONC.
- Preserve comments and formatting behavior through configurable policies.
- Integrate with both xmake and CMake.

## ✨ Features

- Header-only library (no core binary target required).
- C++23.
- JSON and JSONC file loading/saving.
- Strong typed mapping with support for aggregates.
- Field comments via `annotated<T, ...>`.
- Field renaming via `renamed<T, "alias">`.
- Numeric clamping via `ranged<T, Min, Max>`.
- Custom type support via `sculk::reflection::jsonc::serializer<T>` specialization.
- Rewrite policies for format/recovery workflows.

## 📁 Project Layout

- `include/`: public headers
- `test/`: examples and tests
- `xmake.lua`: xmake build entry
- `CMakeLists.txt`: CMake build entry

## 📦 Dependencies

The repository currently uses:

- `JSONC` v1.4.0
- `boost_pfr` 2.2.0
- `magic_enum` v0.9.7

In CMake integration, dependencies are pulled with `FetchContent`.

## 🚀 Quick Start

### Include

```cpp
#include "sculk/reflection/jsonc/reflection.hpp"
```

### Minimal Example

```cpp
#include "sculk/reflection/jsonc/reflection.hpp"

struct AppConfig {
    sculk::reflection::annotated<std::string, "host comment"> host{"127.0.0.1"};
    sculk::reflection::ranged<int, 1, 65535>                   port{8080};
    sculk::reflection::renamed<std::string, "log-level">      level{"Info"};
    bool                                                        enable_metrics{true};
};

int main() {
    AppConfig cfg{};

    sculk::reflection::options opts{};
    opts.indent = 4;
    opts.allow_trailing_comma = true;
    opts.keep_extra_comments = true;

    // Read if file exists; generate defaults if missing
    sculk::reflection::jsonc::load_file(cfg, "./config.jsonc", opts);

    // Explicit save
    sculk::reflection::jsonc::save_file(cfg, "./config.jsonc", opts);
}
```

## 🛠️ Custom Serialization

```cpp
struct UserId {
    int value{};
};

namespace sculk::reflection::jsonc {
template <>
struct serializer<UserId> {
    static int to_int(const UserId& v) noexcept { return v.value; }
    static std::optional<UserId> from_int(int v) noexcept { return UserId{v}; }
};
} // namespace sculk::reflection::jsonc
```

## 🔧 CMake Integration

### FetchContent (Recommended)

```cmake
include(FetchContent)

FetchContent_Declare(
    reflection
    GIT_REPOSITORY https://github.com/SculkCatalystMC/Reflection.git
    GIT_TAG main
)
FetchContent_MakeAvailable(reflection)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE reflection::reflection)
```

## ⚡ xmake Integration

```lua
add_repositories("sculk-repo https://github.com/SculkCatalystMC/xmake-repo.git")

add_requires("reflection")
```

## ⚙️ Common Options

Useful fields in `sculk::reflection::options`:

- `indent`: indentation width
- `ignore_comments`: ignore comments during parse/dump
- `allow_trailing_comma`: allow trailing commas
- `keep_extra_comments`: keep non-schema comments while rewriting
- `float_keep_precision`: preserve floating-point precision
- `rewrite_policy`: `always` / `format` / `error` / `never`

## 🧪 Build

### xmake

```bash
xmake f -m release
xmake
```

### CMake

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## 📜 License

This project is licensed under **MPL-2.0**. See `LICENSE`.

Key MPL-2.0 obligations (high level):

- If you modify MPL-covered files and distribute binaries or source, you must make the modified source of those files available under MPL-2.0.
- Keep existing copyright, license, and notice headers.
- Include a copy of the MPL-2.0 license text when redistributing.
- MPL is file-level copyleft: your larger project can use a different license, but modified MPL files remain under MPL-2.0.

For legal certainty, review the full license text.

## 💬 Feedback and Contributing

- Report bugs or request features via Issues: https://github.com/SculkCatalystMC/Reflection/issues
- Contributions are welcome via Pull Requests: https://github.com/SculkCatalystMC/Reflection/pulls

Please include reproducible steps, environment details, and tests when possible.