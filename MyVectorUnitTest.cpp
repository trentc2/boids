#include "gtest/gtest.h"
#include "myvector.h"
#include "myvector.cpp"

TEST(TestSettingX, WhenSettingXExpectEqual)
{
    MyVector vectorTest(2.5, 2.5, 2.5);
    MyVector goldVector(70, 2.5, 2.5);
    vectorTest.set_x(70);
    EXPECT_TRUE(vectorTest == goldVector);
}

TEST(TestGettingX, WhenGettingX_Expect5)
{
    MyVector vectorTest(5, 2.5, 2.5);
    double goldDigit{5};
    EXPECT_TRUE(vectorTest.get_x() ==goldDigit);
}

TEST(TestSettingY, WhenSettingYExpectEqual)
{
    MyVector vectorTest(2.5, 2.5, 2.5);
    MyVector goldVector(2.5, 70, 2.5);
    vectorTest.set_y(70);
    EXPECT_TRUE(vectorTest == goldVector);
}

TEST(TestGettingY, WhenGettingY_Expect2_5)
{
    MyVector vectorTest(5, 2.5, 2.5);
    double goldDigit{2.5};
    EXPECT_TRUE(vectorTest.get_y() == goldDigit);
}

TEST(TestSettingZ, WhenSettingZExpectEqual)
{
    MyVector vectorTest(2.5, 2.5, 2.5);
    MyVector goldVector(2.5, 2.5, 70);
    vectorTest.set_z(70);
    EXPECT_TRUE(vectorTest == goldVector);
}

TEST(TestGettingZ, WhenGettingZ_Expect5)
{
    MyVector vectorTest(2, 2.5, 5);
    double goldDigit{5};
    EXPECT_TRUE(vectorTest.get_z()== goldDigit);
}

TEST(TestDotProduct, WhenDotting2_3_4and_5_6_7expect56)
{
    MyVector v1(2, 3, 4);
    MyVector v2(5, 6, 7);
    double goldDigit{56};
    EXPECT_TRUE(dot(v1, v2) == goldDigit);
}

TEST(TestDotProduct, WhenDotting10_5_6_111and_5_6_7expectNotEqual)
{
    MyVector v1(10.5, 6, 111);
    MyVector v2(5, 6, 7);
    double goldDigit{56};
    EXPECT_FALSE(dot(v1, v2) == goldDigit);
}

TEST(TestMagnitude, WhenGettingMagnitude5_8_9_expect18_97)
{
    MyVector v1(5, 8, 9);
    double goldDigit{sqrt(25+64+81)};
    EXPECT_TRUE(v1.magnitude() == goldDigit);
}

TEST(TestCrossProduct, WhenCrossing_ExpectEqual)
{
    MyVector v1(10.5, 6, 111);
    MyVector v2(5, 6, 7);
    MyVector goldDigit(-624, 481.5, 33);
    EXPECT_TRUE(cross(v1, v2) == goldDigit);
}

TEST(TestNormalize, WhenNormalizeing_55_62_17Expect)
{
    MyVector v1(55, 62, 17);
    v1.normalize();
    MyVector goldVector(0.6500800561098412, 0.7328175177965482, 0.20093383552486);
    EXPECT_TRUE(v1 == goldVector);
}

TEST(TestDistanceBetweenPoint, WhenTesting0_0_0and_5_5_5Expectsqrt45)
{
    MyVector v1(0, 0, 0);
    MyVector v2(5, 5, 5);
    double distance{MyVector::distance_between(v1, v2)};
    double goldDistance{pow(75, 0.5)};
    EXPECT_EQ(distance, goldDistance);
}

TEST(TestDistanceBetweenPoint, WhenTesting45_50_55and_5_5_5Expect78)
{
    MyVector v1(45, 50, 55);
    MyVector v2(5, 5, 5);
    double distance{MyVector::distance_between(v1, v2)};
    double goldDistance{pow(6125, .5)};
    EXPECT_EQ(distance, goldDistance);
}

TEST(TestMagnitudeDivideByZero, WhenTestingDividby0_Expect0)
{
    MyVector v1(0, 0, 0);
    EXPECT_EQ(v1.magnitude(), 0);
}

TEST(TestNormalizationWhenZeros, WhenTestingDividby0_Expect0)
{
    MyVector v1(0, 0, 0);
    v1.normalize();
    EXPECT_EQ(v1.magnitude(), 0);
}

TEST(TestOperatorOverload_plus_equal, WhenAddingVectorToCurrentExpectEqual)
{
    MyVector v1(0, 0, 0);
    MyVector v2(1, 1, 1);
    v1 += v2;
    EXPECT_TRUE(v1 == v2);
}

TEST(TestOperatorOverload_plus_equal2, WhenAddingVectorToCurrentExpectEqual)
{
    MyVector v1(0, 0, 0);
    MyVector v2(-1, -1, -1);
    v1 += v2;
    EXPECT_TRUE(v1 == v2);
}
