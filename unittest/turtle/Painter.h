/**
 * @file Painter.h
 * @brief Painter class
 * @author [Jason Bao](https://github.com/jbaoys)
 */

#pragma once

#include "Turtle.h"

class Painter {
   public:
    Painter(Turtle& turtle, int width, int high)
    : turtle_(turtle)
    , canvasWidth(width)
    , canvasHigh(high)
    , posX(0)
    , posY(0) {}

    /**
     * Draw a circle
     * @param x center x coordinate
     * @param y center y coordinate
     * @param radius radius of the circle
     * @return true if the circle is drawn successfully
     * @return false if the circle is not drawn
     */
    bool DrawCircle(int x, int y, int radius) {
        if (radius <= 0) {
            return false;
        } else if (x < radius || x >= (canvasWidth-radius) || y < 0 || y >= (canvasHigh-2*radius)) {
            return false;
        } else {
            turtle_.PenUp();
            turtle_.GoTo(x, y);
            turtle_.PenDown();
            for (int i = 0; i < 360; i++) {
                posX = x + radius * sin(i * 3.1415926 / 180);
                posY = y + radius * (1 - cos(i * 3.1415926 / 180));
                turtle_.GoTo(posX, posY);
            }
            turtle_.PenUp();
        }
        return true;
    }

    bool DrawNextStroke(Point p) {
        int x = p.x;
        int y = p.y;

        Point currentPos;
        if (!turtle_.GetPosition(currentPos)) {
           return false; 
        }
        if (currentPos.x == x && currentPos.y == y) {
            return false;
        } else if (x < 0 || x >= canvasWidth || y < 0 || y >= canvasHigh) {
            return false;
        } else if (turtle_.IsPenDown()) {
            turtle_.GoTo(x, y);
            posX = x;
            posY = y;
            return true;
        } else {
            return false;
        }
    }

    bool drawMultipleStrokes(std::vector<Point> points) {
        for (int i = 0; i < static_cast<int>(points.size()); i++) {
            if (!DrawNextStroke(points[i])) {
                return false;
            }
        }
        return true;
    }
   private:
    Turtle& turtle_; //!< turtle
    int canvasWidth;
    int canvasHigh;
    int posX;
    int posY;
};

