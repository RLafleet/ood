﻿#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <iostream>
#include <string>
#include "Point.h"
#include "Color.h"
#include "Shape.h"

class CEllipse : public Shape
{
public:
    constexpr static const char* type = "ellipse";

    CEllipse(Color color, Point center, double horizontalRadius, double verticalRadius) :
        Shape(color), m_center(center), m_horizontalRadius(horizontalRadius), m_verticalRadius(verticalRadius) {}

    void Draw(gfx::ICanvas& canvas) const override
    {
        canvas.SetColor(convertColorToHEX(GetColor()));
        canvas.DrawEllipse(m_center.x, m_center.y, m_horizontalRadius, m_verticalRadius);
    }

    Point GetCenter() const
    {
        return m_center;
    }

    [[nodiscard]] double GetHorizontalRadius() const
    {
        return m_horizontalRadius;
    }

    [[nodiscard]] double GetVerticalRadius() const
    {
        return m_verticalRadius;
    }

private:
    Point m_center;
    double m_horizontalRadius;
    double m_verticalRadius;
};
#endif //ELLIPSE_H