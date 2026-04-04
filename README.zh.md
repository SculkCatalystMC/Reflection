# Reflection

[English](README.md)

> 🧩 C++23 纯头文件反射库

一个基于 C++23 的纯头文件反射库，面向强类型数据映射与易于扩展的元编程工作流。

当前内置后端提供 JSON 与 JSON with Comments (JSONC) 支持，核心反射模型可扩展到更广泛的后端。

它适合以配置为中心的开发方式：

- 用普通 C++ 结构体定义配置。
- 从 JSON/JSONC 反序列化到强类型对象。
- 将对象序列化回 JSON/JSONC。
- 通过可配置策略保留注释与格式行为。
- 同时支持 xmake 与 CMake 集成。

## ✨ 特性

- 纯头文件，无需构建主库二进制。
- C++23。
- 内置 JSON 与 JSONC 文件读写支持。
- 面向聚合类型的强类型映射。
- 字段注释：`annotated<T, ...>`。
- 字段重命名：`renamed<T, "alias">`。
- 数值约束：`ranged<T, Min, Max>`。
- 自定义类型序列化：特化 `sculk::reflection::jsonc::serializer<T>`。
- 支持格式化/恢复等回写策略。

## 📁 项目结构

- `include/`: 公共头文件
- `test/`: 示例与测试
- `xmake.lua`: xmake 构建入口
- `CMakeLists.txt`: CMake 构建入口

## 📦 依赖

当前仓库使用：

- `JSONC` v1.4.0
- `boost_pfr` 2.2.0
- `magic_enum` v0.9.7

在 CMake 集成场景下，依赖通过 `FetchContent` 自动拉取。

## 🚀 快速开始

### 引入头文件

```cpp
#include "sculk/reflection/jsonc/reflection.hpp"
```

### 最小示例

```cpp
#include "sculk/reflection/jsonc/reflection.hpp"

struct AppConfig {
    sculk::reflection::annotated<std::string, "host 注释"> host{"127.0.0.1"};
    sculk::reflection::ranged<int, 1, 65535>                port{8080};
    sculk::reflection::renamed<std::string, "log-level">   level{"Info"};
    bool                                                     enable_metrics{true};
};

int main() {
    AppConfig cfg{};

    sculk::reflection::options opts{};
    opts.indent = 4;
    opts.allow_trailing_comma = true;
    opts.keep_extra_comments = true;

    // 文件存在时读取，不存在时生成默认配置
    sculk::reflection::jsonc::load_file(cfg, "./config.jsonc", opts);

    // 主动保存
    sculk::reflection::jsonc::save_file(cfg, "./config.jsonc", opts);
}
```

## 🛠️ 自定义序列化

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

## 🔧 CMake 集成

### FetchContent（推荐）

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

## ⚡ xmake 集成

```lua
add_repositories("sculk-repo https://github.com/SculkCatalystMC/xmake-repo.git")

add_requires("reflection")
```

## ⚙️ 常用配置

`sculk::reflection::options` 的常用字段：

- `indent`: 缩进宽度
- `ignore_comments`: 解析/输出时忽略注释
- `allow_trailing_comma`: 允许尾逗号
- `keep_extra_comments`: 回写时尽量保留额外注释
- `float_keep_precision`: 保留浮点精度
- `rewrite_policy`: `always` / `format` / `error` / `never`

## 🧪 构建

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

## 📜 开源协议

本项目采用 **MPL-2.0** 协议，详见 `LICENSE`。

MPL-2.0 核心要求（高层说明）：

- 如果你修改了 MPL 覆盖的文件并进行分发（源码或二进制），必须以 MPL-2.0 公开这些被修改文件的源码。
- 必须保留原有版权、许可和声明信息。
- 再分发时应附带 MPL-2.0 协议文本。
- MPL 属于文件级弱 copyleft：你的整体工程可以使用其他协议，但被修改的 MPL 文件仍需保持 MPL-2.0。

如需法律层面的准确解释，请以协议原文为准。

## 💬 反馈与贡献

- 问题反馈与功能建议（Issues）：https://github.com/SculkCatalystMC/Reflection/issues
- 欢迎提交贡献（Pull Requests）：https://github.com/SculkCatalystMC/Reflection/pulls

建议在反馈中附上复现步骤、环境信息与最小示例；提交 PR 时尽量附带测试。

### 版权所有 © 2026 SculkCatalystMC。保留所有权利。