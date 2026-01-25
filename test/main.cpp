#include "jsonc/reflection.hpp"
#include <filesystem>
#include <fstream>

using namespace jsonc::reflection;

struct TestType {
    TestType() = default;
    TestType(std::string_view data) : data_(data) {}
    std::string data_;
};

namespace jsonc::reflection {
template <>
struct Serializer<TestType> {
    static std::string             to_string(const TestType& t) noexcept { return t.data_; }
    static std::optional<TestType> from_string(std::string_view s) noexcept { return TestType(s); }
};
} // namespace jsonc::reflection

// class TestErrorType {
//     int x_;
// };

enum class TestEnum { AAA = 0, BBB = 1, CCC = 2, DDD = 3 };

enum class TestEnumFlag { AAA = 0, BBB = 1 << 0, CCC = 1 << 1, DDD = 1 << 2, EEE = 5 };

struct Config {
    // TestErrorType                                               test_e  = {}; // compile error
    std::string                                                 test_1  = "test string";
    Annotated<std::string, "xiwhgasdjjhoikwq">                  test_2  = "test string with comments";
    Renamed<Annotated<std::string, "738whdbhahisdS">, "test-3"> test_3  = "test string with renamed key and comments";
    int                                                         test_4  = 232123;
    double                                                      test_5  = 3.526781;
    float                                                       test_6  = 26781.234;
    int16_t                                                     test_7  = -2671;
    std::optional<uint8_t>                                      test_8  = {};
    std::string_view                                            test_9  = "sv test";
    TestType                                                    test_10 = {"test custom type"};
    Ranged<short, -3, 5678>                                     test_11 = 23345;
    TestEnum                                                    test_12 = TestEnum(3);
    TestEnumFlag                                                test_13 = TestEnumFlag(6);
    std::variant<bool, float>                                   test_14 = 2.345f;
    std::tuple<int, int, std::string, float, TestEnum>          test_15 = {234, -3782, "wyuhdsj", 7.492f, TestEnum::BBB};
    std::list<int>                                              test_16 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<std::string>                                    test_17 = {"xxxxx", "yyyyy"};
    std::array<short, 4>                                        test_18 = {22, 33, 44, 55};
};

std::optional<std::string> read_file(std::filesystem::path const& path) {
    if (!std::filesystem::exists(path)) { return std::nullopt; }
    auto file = std::ifstream(path, std::ios_base::in);
    return std::string(std::istreambuf_iterator<char>(file), {});
}

bool write_file(std::filesystem::path const& path, std::string_view content) {
    if (!std::filesystem::exists(path.parent_path())) { std::filesystem::create_directories(path.parent_path()); }
    auto file = std::ofstream(path, std::ios_base::out);
    file << content;
    return true;
}

int main() {
    Annotated<Config, "test config", "xxxxx"> settings;
    // if (auto content = read_file("./test.jsonc"); content) {
    //     if (auto json = jsonc::parse(*content, true); json) { jsonc::reflection::deserialize(settings, *json).value(); }
    // }

    auto res = jsonc::reflection::serialize(settings);
    write_file("./test.jsonc", res.dump());
    return 0;
}