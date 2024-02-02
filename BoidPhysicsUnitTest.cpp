#include "gtest/gtest.h"
#include "boidphysics.h"
#include "boidphysics.cpp"
#include "myvector.cpp"
#include "myvector.h"

TEST(TestRandomPosition, ExpectNumbersBetween1and20_)
{
    BoidPhysics testBoid;
    MyVector pos = testBoid.position;
    EXPECT_GT(pos.get_x(), 0);
}

TEST(TestRandomPosition, ExpectNumbersBetween1and20)
{
    BoidPhysics testBoid;
    MyVector pos = testBoid.position;
    EXPECT_GT(pos.get_y(), 0);
}

TEST(TestRandomPosition, ExpectNumbersBetween1and20__)
{
    BoidPhysics testBoid;
    MyVector pos = testBoid.position;
    EXPECT_GT(pos.get_z(), 0);
}

TEST(TestRandomPosition, ExpectNumbersBetween1and20___)
{
    BoidPhysics testBoid;
    MyVector pos = testBoid.position;
    EXPECT_LE(pos.get_x(), 20);
}

TEST(TestRandomPosition, ExpectNumbersBetween1and20_1_)
{
    BoidPhysics testBoid;
    MyVector pos = testBoid.position;
    EXPECT_LE(pos.get_y(), 20);
}

TEST(TestRandomPosition, ExpectNumbersBetween1and20__1)
{
    BoidPhysics testBoid;
    MyVector pos = testBoid.position;
    EXPECT_LE(pos.get_z(), 20);
}

TEST(TestRandomOrientation, ExpectNumbersBetween_n1_1)
{
    BoidPhysics testBoid;
    MyVector pos = testBoid.orientation;
    EXPECT_GT(pos.get_x(), -1);
}

TEST(TestRandomOrientation, ExpectNumbersBetween_n1_1_)
{
    BoidPhysics testBoid;
    MyVector pos = testBoid.orientation;
    EXPECT_GT(pos.get_x(), -1);
}

TEST(TestRandomOrientation, ExpectNumbersBetween_n1_1__)
{
    BoidPhysics testBoid;
    MyVector pos = testBoid.orientation;
    EXPECT_GT(pos.get_x(), -1);
}

TEST(TestRandomOrientation, ExpectNumbersBetween_n1_1_0_)
{
    BoidPhysics testBoid;
    MyVector pos = testBoid.orientation;
    EXPECT_LT(pos.get_x(), 1);
}

TEST(TestRandomOrientation, ExpectNumbersBetween_n1_1__0)
{
    BoidPhysics testBoid;
    MyVector pos = testBoid.orientation;
    EXPECT_LT(pos.get_x(), 1);
}

TEST(TestRandomOrientation, ExpectNumbersBetween_n1_1__0_)
{
    BoidPhysics testBoid;
    MyVector pos = testBoid.orientation;
    EXPECT_LT(pos.get_x(), 1);
}

TEST(TestNewPosition, Expect_VectorTrue)
{
    MyVector firstPosition(5, 3, 2);
    MyVector firstOrientation(1, 1, 1);
    firstOrientation.normalize();
    BoidPhysics testBoid(firstPosition, firstOrientation);
    testBoid.calculate_new_position(1, 5);
    MyVector goldVector(5/pow(3, 0.5) + 5*pow(3, 0.5)/pow(3, 0.5), 5/pow(3, 0.5) + 3*pow(3, 0.5)/pow(3, 0.5), 5/pow(3, 0.5) + 5*pow(3, 0.5)/pow(2, 0.5));
    EXPECT_TRUE(goldVector == testBoid.newPosition);
}

TEST(TestNewPosition_and_UPdate, Expect_Vector_of_True)
{
    MyVector firstPosition(5, 3, 2);
    MyVector firstOrientation(1, 1, 1);
    firstOrientation.normalize();
    BoidPhysics testBoid(firstPosition, firstOrientation);
    testBoid.calculate_new_position(1, 5);
    testBoid.update_position_and_orientation();
    MyVector goldVector(5/pow(3, 0.5) + 5*pow(3, 0.5)/pow(3, 0.5), 5/pow(3, 0.5) + 3*pow(3, 0.5)/pow(3, 0.5), 5/pow(3, 0.5) + 5*pow(3, 0.5)/pow(2, 0.5));
    EXPECT_TRUE(goldVector == testBoid.position);
}

TEST(TestCheckBoxXBox, WhenXisCloseToWallExpectOrientationToChange)
{
    MyVector currentPosition(1, 1, 1);
    MyVector currentOrientation(1, 1, 1);
    BoidPhysics testBoid(currentPosition, currentOrientation);
    testBoid.check_x_wall();
    EXPECT_NE(testBoid.newOrientation.get_x(), currentOrientation.get_x());
}

TEST(TestCheckBoxYBox, WhenYisCloseToWallExpectOrientationToChange)
{
    MyVector currentPosition(1, 1, 1);
    MyVector currentOrientation(1, 1, 1);
    BoidPhysics testBoid(currentPosition, currentOrientation);
    testBoid.check_y_wall();
    EXPECT_NE(testBoid.newOrientation.get_y(), currentOrientation.get_y());
}
