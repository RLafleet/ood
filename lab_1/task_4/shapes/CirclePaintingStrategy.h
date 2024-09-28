#include <iostream>
#include <string>
#include "IDrawable.h"

namespace shapes
{
    class CirclePaintingStrategy : public IDrawable
    {
    public:
        CirclePaintingStrategy(
            double centerX,
            double centerY,
            double radius
        ) :
            m_centerX(centerX), m_centerY(centerY), m_radius(radius) {}

        void Draw(gfx::ICanvas& canvas, uint32_t color) const override
        {
            canvas.SetColor(color);
            canvas.DrawEllipse(m_centerX, m_centerY, m_radius, m_radius);
        }

        void Move(double dx, double dy) override
        {
            m_centerX += dx;
            m_centerY += dy;
        }

        [[nodiscard]] std::string GetType() const override
        {
            return m_type;
        }

        [[nodiscard]] std::string ToString() const override
        {
            std::ostringstream oss;
            oss << m_centerX << " " << m_centerY << " " << m_radius;
            return oss.str();
        }

    private:
        // static избавить от дублирования
        const std::string m_type = "circle";
        double m_centerX;
        double m_centerY;
        double m_radius;
    };
}
