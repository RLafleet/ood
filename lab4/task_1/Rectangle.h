#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iostream>
#include <string>
#include "Point.h"
#include "Color.h"
#include "Shape.h"

class Rectangle : public Shape
{
public:
    constexpr static const char* type = "rectangle";

    Rectangle(Color color, Point leftTop, double width, double height) :
        Shape(color), m_leftTop(leftTop), m_width(width), m_height(height) {}

    void Draw(gfx::ICanvas& canvas) const override
    {
        canvas.SetColor(convertColorToHEX(GetColor()));
        canvas.DrawLine(m_leftTop.x, m_leftTop.y, m_leftTop.x + m_width, m_leftTop.y);
        canvas.DrawLine(m_leftTop.x + m_width, m_leftTop.y, m_leftTop.x + m_width, m_leftTop.y + m_height);
        canvas.DrawLine(m_leftTop.x + m_width, m_leftTop.y + m_height, m_leftTop.x, m_leftTop.y + m_height);
        canvas.DrawLine(m_leftTop.x, m_leftTop.y + m_height, m_leftTop.x, m_leftTop.y);
    }

    [[nodiscard]] Point GetLeftTop() const
    {
        return m_leftTop;
    }

    [[nodiscard]] double GetWidth() const
    {
        return m_width;
    }

    [[nodiscard]] double GetHeight() const
    {
        return m_height;
    }

private:
    Point m_leftTop;
    double m_width;
    double m_height;
};

#endif //RECTANGLE_H