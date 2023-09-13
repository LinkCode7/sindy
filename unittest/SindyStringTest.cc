#include <gtest/gtest.h>

#include "../sindy/SindyString.h"

TEST(string, remove_annotation)
{
    EXPECT_TRUE(sindy::removeAnnotation("") == "");
    EXPECT_TRUE(sindy::removeAnnotation(" ") == " ");
    EXPECT_TRUE(sindy::removeAnnotation("ss") == "ss");
    EXPECT_TRUE(sindy::removeAnnotation("123") == "123");
    EXPECT_TRUE(sindy::removeAnnotation(" 1  2  3 ") == " 1  2  3 ");
    EXPECT_TRUE(sindy::removeAnnotation("千里江陵一日还") == "千里江陵一日还");

    EXPECT_TRUE(sindy::removeAnnotation("/*轻舟已过万重山*/") == "");
    EXPECT_TRUE(sindy::removeAnnotation("/*hello*/ world") == " world");
    EXPECT_TRUE(sindy::removeAnnotation("/*hello*/ /*world*/") == " ");
    EXPECT_TRUE(sindy::removeAnnotation("/*轻舟已过*/万重山") == "万重山");
    EXPECT_TRUE(sindy::removeAnnotation("// ss") == "");
    EXPECT_TRUE(sindy::removeAnnotation("sf/**/ ") == "sf ");

    EXPECT_TRUE(sindy::removeAnnotation("/*asd*/-// /*asd*/") == "-");

    std::string fun("void fun(int a/* name*/, double b/*  test   */, std::string const& str/*string val*/)");
    EXPECT_TRUE(sindy::removeAnnotation(fun) == "void fun(int a, double b, std::string const& str)");

    EXPECT_TRUE(sindy::removeAnnotation("/*hello*//*world*/") == "");

    EXPECT_TRUE(sindy::removeAnnotation("specific/*aa//aa*//*b//b*/ // note") == "specific ");
}
