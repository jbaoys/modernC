/**
 * @file MockTurtleTest.cc
 * @brief Unit tests for MockTurtle
 * @author [Jason Bao](https://github.com/jbaoys)
 */

#include "MockTurtle.h"
#include "Painter.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::AtLeast;                           // #1
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using ::testing::_;
using ::testing::DoAll;

/**
 * Turtle::GetPosition() action: Set point using gmock action macro
 * @param arg0: Point to set. This is a reference of Turtle::GetPosition()
 *              argument.
 * @param x: x coordinate
 * @param y: y coordinate
 * @return true
 */
ACTION_P2(setPoint, x,  y) {
    arg0.x = x;
    arg0.y = y;
    return true;
}

MATCHER_P(PointEq, x, "") {
    return arg == x;
}

MATCHER_P(PointNe, x, "") {
    return arg != x;
}

TEST(PainterTest, CanDrawSomething) {
    MockTurtle turtle;                              // #2
    EXPECT_CALL(turtle, PenDown())                  // #3
      .Times(AtLeast(1));
    EXPECT_CALL(turtle, GoTo(_,_)).Times(AtLeast(1));
    EXPECT_CALL(turtle, PenUp()).Times(AtLeast(1));

    Painter painter(turtle, 100, 100);              // #4

    EXPECT_FALSE(painter.DrawCircle(0, 0, 10));      // #5
    EXPECT_TRUE(painter.DrawCircle(10, 0, 10));      // #5
}

TEST(PainterTest, DrawNextStroke) {
    NiceMock<MockTurtle> turtle;

    // Use lambda expression to set point
    EXPECT_CALL(turtle, GetPosition(_)).WillOnce([](Point& p) { p = Point(11, 22); return true; });
    EXPECT_CALL(turtle, IsPenDown()).WillOnce(Return(false));
    Painter painter(turtle, 100, 100);
    EXPECT_FALSE(painter.DrawNextStroke(Point(10,20)));
}

TEST(PainterTest, DrawNextStroke1) {
    NiceMock<MockTurtle> turtle;

    // Use gmock action macro to set point
    EXPECT_CALL(turtle, GetPosition(_)).WillOnce(setPoint(11, 22));
    EXPECT_CALL(turtle, IsPenDown()).WillOnce(Return(false));
    Painter painter(turtle, 100, 100);
    EXPECT_FALSE(painter.DrawNextStroke(Point(10,20)));
}

TEST(PainterTest, DrawNextStroke2) {
    NiceMock<MockTurtle> turtle;

    EXPECT_CALL(turtle, GetPosition(_)).WillOnce(setPoint(11, 22));
    EXPECT_CALL(turtle, GoTo(_,_)).Times(AtLeast(1));
    EXPECT_CALL(turtle, IsPenDown()).WillOnce(Return(true));
    Painter painter(turtle, 100, 100);
    EXPECT_TRUE(painter.DrawNextStroke(Point(10,20)));
}

TEST(PainterTest, DrawMutilpleStrokes) {
    NiceMock<MockTurtle> turtle;
    EXPECT_CALL(turtle, GetPosition(_)).WillRepeatedly(setPoint(1, 2));
    // Use the PointEq and PointNe matcher to rule out only expected point - (10,20)
    EXPECT_CALL(turtle, GoTo(PointEq(10), PointEq(20))).Times(1);
    EXPECT_CALL(turtle, GoTo(PointNe(10), PointNe(20))).Times(AtLeast(1));

    EXPECT_CALL(turtle, IsPenDown()).WillRepeatedly(Return(true));
    Painter painter(turtle, 100, 100);
    std::vector<Point> points = {Point(10, 20), Point(11, 21), Point(12, 22), Point(13, 23), Point(14, 24), Point(15, 25), Point(16, 26)};
    EXPECT_TRUE(painter.drawMultipleStrokes(points));
}
// int main( int argc, char *argv[] ) {
//     ::testing::InitGoogleMock( &argc, argv );
//     return RUN_ALL_TESTS( );
// }
