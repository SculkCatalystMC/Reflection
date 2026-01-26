#include "jsonc/reflection.hpp"

using namespace jsonc::reflection;

struct TestType1 {
    TestType1() = default;
    TestType1(std::string_view data) : data_(data) {}
    std::string data_;
};

struct TestType2 {
    TestType2() = default;
    TestType2(std::string_view data) : data_(data) {}

    operator std::string() const noexcept { return data_; }

    std::string data_;
};

namespace jsonc::reflection {
template <>
struct Serializer<TestType1> {
    static float                    to_float(const TestType1&) noexcept { return -1.23f; }
    static std::optional<TestType1> from_float(float) noexcept { return TestType1{}; }
};
} // namespace jsonc::reflection

enum class TestEnum { AAA = 0, BBB = 1, CCC = 2, DDD = 3 };

enum class TestEnumFlag { AAA = 0, BBB = 1 << 0, CCC = 1 << 1, DDD = 1 << 2, EEE = BBB | DDD };

struct Config {
    std::string                                                 test_1  = "test string";
    Annotated<std::string, "xiwhgasdjjhoikwq">                  test_2  = "test string with comments";
    Renamed<Annotated<std::string, "738whdbhahisdS">, "test-3"> test_3  = "test string with renamed key and comments";
    int                                                         test_4  = 232123;
    double                                                      test_5  = 3.526781;
    float                                                       test_6  = 26781.234;
    int16_t                                                     test_7  = -2671;
    std::optional<uint8_t>                                      test_8  = {};
    std::string_view                                            test_9  = "sv test";
    TestType1                                                   test_10 = {"test custom type 1"};
    Ranged<short, -3, 5678>                                     test_11 = 23345;
    TestEnum                                                    test_12 = TestEnum(3);
    TestEnumFlag                                                test_13 = TestEnumFlag(6);
    std::variant<bool, float>                                   test_14 = 2.345f;
    std::tuple<int, int, std::string, float, TestEnum>          test_15 = {234, -3782, "wyuhdsj", 7.492f, TestEnum::BBB};
    std::list<int>                                              test_16 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<std::string>                                    test_17 = {"xxxxx", "yyyyy"};
    std::array<short, 4>                                        test_18 = {22, 33, 44, 55};
    bool                                                        test_19 = false;
    std::unordered_map<int16_t, float>                          test_20 = {
        {{1, 2.3f}, {4, 5.32f}, {23, 2.45f}}
    };
    std::map<TestEnum, std::variant<bool, int, std::string>> test_21 = {
        {{TestEnum::AAA, false}, {TestEnum::BBB, 123456}, {TestEnum::CCC, "test 21"}}
    };
    TestType2 test_22 = {"test custom type 2"};
    struct {
        int                         xxxxx = 123;
        Annotated<double, "double"> yyyyy = 64738.543;
    } test_23;
};

int main() {
    Annotated<Config, "test config", "xxxxx"> settings;
    jsonc::reflection::load_config(settings, "./test.jsonc");
    return 0;
}