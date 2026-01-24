#include "jsonc-reflection/Reflection.hpp"
#include <filesystem>
#include <fstream>

using namespace jsonc_reflection;

struct TestType {
    std::string data_;

    TestType() = default;
    TestType(std::string_view data) : data_(data) {}

    std::string to_string() const { return data_; }

    static TestType FromString(std::string_view data) { return TestType(data); }
};

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

std::optional<std::string> readFile(std::filesystem::path const& filePath, bool isBinary = false) {
    if (!std::filesystem::exists(filePath)) { return std::nullopt; }
    std::ifstream           fRead;
    std::ios_base::openmode mode = std::ios_base::in;
    if (isBinary) mode |= std::ios_base::binary;
    fRead.open(filePath, mode);
    if (!fRead.is_open()) { return std::nullopt; }
    std::string data((std::istreambuf_iterator<char>(fRead)), {});
    fRead.close();
    return data;
}

bool writeFile(std::filesystem::path const& filePath, std::string_view content, bool isBinary = false) {
    std::ofstream           fWrite;
    std::ios_base::openmode mode = std::ios_base::out;
    if (isBinary) mode |= std::ios_base::binary;
    if (!std::filesystem::exists(filePath.parent_path())) { std::filesystem::create_directories(filePath.parent_path()); }
    fWrite.open(filePath, mode);
    if (!fWrite.is_open()) { return false; }
    fWrite << content;
    fWrite.close();
    return true;
}

int main() {
    Config settings;
    // if (auto content = readFile("./test.jsonc"); content) {
    //     if (auto json = jsonc::parse(*content, true); json) { jsonc_reflection::deserialize(settings, *json).value(); }
    // }
    auto res = jsonc_reflection::serialize(settings);
    writeFile("./test.jsonc", res.dump());
    return 0;
}