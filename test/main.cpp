#include "jsonc/reflection.hpp"
#include <list>
#include <print>
#include <set>

namespace jr = jsonc::reflection;

struct TestType1 {
    TestType1() = default;
    TestType1(float data) : data_(data) {}
    float data_;
};

struct TestType2 {
    TestType2() = default;
    TestType2(std::string_view data) : data_(data) {}
    std::string data_;
};

namespace jsonc::reflection {
template <>
struct serializer<TestType1> {
    static float                    to_float(const TestType1& t) noexcept { return t.data_; }
    static std::optional<TestType1> from_float(float v) noexcept { return TestType1{v}; }
};
template <>
struct serializer<TestType2> {
    static std::string              to_any_number(const TestType2& t) noexcept { return t.data_; }
    static std::optional<TestType2> from_any_number(std::string_view v) noexcept { return TestType2{v}; }
};
} // namespace jsonc::reflection

enum class TestEnum { AAA = 0, BBB = 1, CCC = 2, DDD = 3 };

enum class TestEnumFlag { AAA = 0, BBB = 1 << 0, CCC = 1 << 1, DDD = 1 << 2, EEE = BBB | DDD };

struct Test26 {
    void call(std::string_view val) noexcept { std::println("value: {}", val); }
};

struct Test27 {
    void call() noexcept { std::println("auto save"); }
};

struct Config {
    std::string                                                         test_1  = "test string";
    jr::annotated<std::string, "xiwhgasdjjhoikwq">                      test_2  = "test string with comments";
    jr::renamed<jr::annotated<std::string, "738whdbhahisdS">, "test-3"> test_3  = "test string with renamed key and comments";
    int                                                                 test_4  = 232123;
    double                                                              test_5  = 3.526781;
    float                                                               test_6  = 26781.234f;
    int16_t                                                             test_7  = -2671;
    std::optional<uint8_t>                                              test_8  = {123};
    std::string                                                         test_9  = "str test";
    TestType1                                                           test_10 = {123.456f};
    jr::ranged<short, -3, 5678>                                         test_11 = 23345;
    TestEnum                                                            test_12 = TestEnum(3);
    TestEnumFlag                                                        test_13 = TestEnumFlag(6);
    std::variant<bool, float, int, uint64_t>                            test_14 = 2.345f;
    std::tuple<int, int, std::string, float, TestEnum>                  test_15 = {234, -3782, "wyuhdsj", 7.492f, TestEnum::BBB};
    std::list<jr::annotated<int, "xwsaijnjhiknjk">>                     test_16 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<std::string>                                            test_17 = {"xxxxx", "yyyyy"};
    std::array<short, 4>                                                test_18 = {22, 33, 44, 55};
    bool                                                                test_19 = false;
    std::unordered_map<float, float>                                    test_20 = {
        {{1.0f, 2.3f}, {4.3f, 5.32f}, {23.0f, 2.45f}}
    };
    std::map<TestEnum, std::variant<bool, int, std::string>> test_21 = {
        {{TestEnum::AAA, false}, {TestEnum::BBB, 123456}, {TestEnum::CCC, "test 21"}}
    };
    TestType2 test_22 = {"123456787654323456789824324543454323456765432345676543765432345678"};
    struct {
        int                             xxxxx = 123;
        jr::annotated<double, "double"> yyyyy = 64738.543;
    } test_23;
    std::set<int>                                  test_24 = {1, 23, 456};
    std::vector<jsonc::ordered_jsonc::object_type> test_25{};
    jr::dispatcher<std::string, Test26>            test_26{};
    jr::dispatcher<int, Test27>                    test_27{};
};

int main() {
    jr::annotated<Config, "test config", "xxxxx"> settings;
    jr::load_file(settings, "./test.jsonc");
    return 0;
}
