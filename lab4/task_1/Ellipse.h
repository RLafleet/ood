#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <iostream>
#include <string>
#include "Point.h"
#include "Color.h"
#include "CShape.h"

class Ellipse : public CShape
{
public:
    constexpr static const char* type = "ellipse";

    Ellipse(Color color, Point center, double horizontalRadius, double verticalRadius) :
        CShape(color), m_center(center), m_horizontalRadius(horizontalRadius), m_verticalRadius(verticalRadius) {}

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