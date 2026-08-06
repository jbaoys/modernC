/**
 * @file Turtle.h
 * @brief Header file for the Turtle class
 */
#pragma once
/**
 * @class Turtle
 * @brief Interface for the Turtle class
 * @details This class defines the interface for the Turtle class
 * @author [Jason Bao](https://github.com/jbaoys)
 * @date 2024-04-20
 * @copyright Copyright (c) 2024
 */
struct Point {
    Point() : x(0), y(0) {}
    Point(int xx, int yy) : x(xx), y(yy) {}
    int x;
    int y;
};
class Turtle {
public:
    Turtle() {}
    virtual ~Turtle() {}
    virtual void PenUp() = 0;
    virtual void PenDown() = 0;
    virtual void Forward(int distance) = 0;
    virtual void Turn(int degrees) = 0;
    virtual void GoTo(int x, int y) = 0;
    virtual void Circle(int radius) = 0;
    virtual int GetX() const = 0;
    virtual int GetY() const = 0;
    virtual bool GetPosition(Point &point) const = 0;
    virtual bool IsPenDown() const = 0;
};
