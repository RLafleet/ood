#ifndef INC_4_TRIANGLE_H
#define INC_4_TRIANGLE_H

#include <iostream>
#include <string>
#include "IDrawable.h"
#include "Point.h" 

namespace shapes
{
    class TrianglePaintingStrategy : public IDrawable
    {
    public:
        TrianglePaintingStrategy(
            const Point& p1,
            const Point& p2,
            const Point& p3
        ) :
            m_point1(p1), m_point2(p2), m_point3(p3) {}

        void Draw(gfx::ICanvas& canvas, uint32_t color) const override
        {
            canvas.SetColor(color);
            canvas.MoveTo(m_point1.GetX(), m_point1.GetY());
            canvas.LineTo(m_point2.GetX(), m_point2.GetY());
            canvas.LineTo(m_point3.GetX(), m_point3.GetY());
            canvas.LineTo(m_point1.GetX(), m_point1.GetY());
        }

        void Move(double dx, double dy) override
        {
            m_point1.Move(dx, dy);
            m_point2.Move(dx, dy);
            m_point3.Move(dx, dy);
        }

        [[nodiscard]] std::string GetType() const override
        {
            return m_type;
        }

        [[nodiscard]] std::string ToString() const override
        {
            std::ostringstream oss;
            oss << m_point1.GetX() << " "
                << m_point1.GetY() << " "
                << m_point2.GetX() << " "
                << m_point2.GetY() << " "
                << m_point3.GetX() << " "
                << m_point3.GetY();
            return oss.str();
        }

    private:
        const std::string m_type = "triangle";
        Point m_point1;
        Point m_point2;
        Point m_point3;
    };
}

#endif //INC_4_TRIANGLE_H