#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include <string>
#include "Point.h"
#include "Color.h"
#include "Shape.h"

class CTriangle : public Shape
{
public:
    constexpr static const char* type = "triangle";

    CTriangle(Color color, Point point1, Point point2, Point point3) :
        Shape(color), m_point1(point1), m_point2(point2), m_point3(point3) {}

    void Draw(gfx::ICanvas& canvas) const override
    {
        canvas.SetColor(convertColorToHEX(GetColor()));
        canvas.DrawLine(m_point1.x, m_point1.y, m_point2.x, m_point2.y);
        canvas.DrawLine(m_point2.x, m_point2.y, m_point3.x, m_point3.y);
        canvas.DrawLine(m_point3.x, m_point3.y, m_point1.x, m_point1.y);
    }

    Point GetPoint1() const
    {
        return m_point1;
    }

    Point GetPoint2() const
    {
        return m_point2;
    }

    Point GetPoint3() const
    {
        return m_point3;
    }

private:
    Point m_point1;
    Point m_point2;
    Point m_point3;
};

#endif //TRIANGLE_H