#include <gtest/gtest.h>

#include "../sindy/SindyMath.h"

#ifndef PI
    #define PI 3.14159265358979323846    // pi
    #define PI2 6.28318530717958623200   // pi*2
    #define PI_2 1.57079632679489661923  // pi/2
    #define PI_4 0.785398163397448309616 // pi/4
#endif

// 3.1415926 5358 9793 23846
TEST(maths, roundFloat)
{
    double pi = sindy::roundFloat(PI, 10);
    EXPECT_NEAR(3.1415926536, pi, 0.000000001);
}

TEST(maths, simplifyFloat)
{
    EXPECT_TRUE(sindy::simplifyFloat(0.0, 0) == "0");
    EXPECT_TRUE(sindy::simplifyFloat(0.0, 1) == "0");
    EXPECT_TRUE(sindy::simplifyFloat(0.0, 2) == "0");
    EXPECT_TRUE(sindy::simplifyFloat(0.0, 3) == "0");
    EXPECT_TRUE(sindy::simplifyFloat(0.005, 2) == "0.01");

    EXPECT_TRUE(sindy::simplifyFloat(100.0, 3) == "100");
    EXPECT_TRUE(sindy::simplifyFloat(993.0, 3) == "993");

    EXPECT_TRUE(sindy::simplifyFloat(66.4, 0) == "66");
    EXPECT_TRUE(sindy::simplifyFloat(66.5, 0) == "67");
    EXPECT_TRUE(sindy::simplifyFloat(66.6, 0) == "67");

    EXPECT_TRUE(sindy::simplifyFloat(0.0001, 0) == "0");
    EXPECT_TRUE(sindy::simplifyFloat(0.0001, 1) == "0");
    EXPECT_TRUE(sindy::simplifyFloat(0.0001, 2) == "0");
    EXPECT_TRUE(sindy::simplifyFloat(0.0001, 3) == "0");
    EXPECT_TRUE(sindy::simplifyFloat(0.0001, 4) == "0.0001");
    EXPECT_TRUE(sindy::simplifyFloat(0.0001, 5) == "0.0001");

    EXPECT_TRUE(sindy::simplifyFloat(0.00110, 3) == "0.001");
    EXPECT_TRUE(sindy::simplifyFloat(0.00150, 3) == "0.002");
    EXPECT_TRUE(sindy::simplifyFloat(0.00960, 3) == "0.01");
    EXPECT_TRUE(sindy::simplifyFloat(0.09960, 3) == "0.1");
    EXPECT_TRUE(sindy::simplifyFloat(0.99960, 3) == "1");
    EXPECT_TRUE(sindy::simplifyFloat(9.99960, 3) == "10");
    EXPECT_TRUE(sindy::simplifyFloat(99.99960, 3) == "100");

    EXPECT_TRUE(sindy::simplifyFloat(PI, 10) == "3.1415926536");
}