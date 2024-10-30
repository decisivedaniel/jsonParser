#include "json_eval.h"
#include "gtest/gtest.h"
namespace {
    class JsonTest : public testing::Test {
        protected:
            JsonTest() {
                std::string testFile = "test.json";
                std::ifstream TestFile(testFile);
                base = JsonFactory().readJson(&TestFile);
                TestFile.close();
            }
            JsonBase base;
    };

    TEST_F(JsonTest, Trivial) {
        EXPECT_EQ(base.eval("a.b[1]"), "2") << "a.b[1] does not return 2";
        EXPECT_EQ(base.eval("a.b[2].c"),"test") << "a.b[2].c does not equal test";
        EXPECT_EQ(base.eval("a.b"), '[ 1, 2, { "c" : "test" } ]');

    }
}