#include "json.h"
#include <gtest/gtest.h>

namespace {
    class JsonTest : public testing::Test {
        protected:
            JsonTest() {
                std::string testFile = "test.json";
                std::ifstream TestFile(testFile);
                base = Json::readJson(testFile);
                TestFile.close();
            }
            std::shared_ptr<Json::Node> base;
    };

    TEST_F(JsonTest, Trivial) {
        EXPECT_EQ(Json::eval(base, std::string("a.b[1]")), std::string("2")) << "a.b[1] does not return 2";
        EXPECT_EQ(Json::eval(base, std::string("a.b[2].c")),std::string("test")) << "a.b[2].c does not equal test";
        EXPECT_EQ(Json::eval(base, std::string("a.b")), std::string("[ 1, 2, { \"c\" : \"test\" } ]"));

    }
}