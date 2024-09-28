#ifndef INC_4_RECTANGLEPAINTINGSTRATEGY_H
#define INC_4_RECTANGLEPAINTINGSTRATEGY_H

#include <iostream>
#include <string>
#include "IDrawable.h"
#include "Point.h" 

namespace shapes {
    class RectanglePaintingStrategy : public IDrawable
    {
    public:
        RectanglePaintingStrategy(
            const Point& topLeft,
            double width,
            double height
        ) :
            m_topLeft(topLeft), m_width(width), m_height(height) {}

        void Draw(gfx::ICanvas& canvas, uint32_t color) const override
        {
            canvas.SetColor(color);
            canvas.MoveTo(m_topLeft.GetX(), m_topLeft.GetY());
            canvas.LineTo(m_topLeft.GetX() + m_width, m_topLeft.GetY());
            canvas.LineTo(m_topLeft.GetX() + m_width, m_topLeft.GetY() + m_height);
            canvas.LineTo(m_topLeft.GetX(), m_topLeft.GetY() + m_height);
            canvas.LineTo(m_topLeft.GetX(), m_topLeft.GetY());
        }

        void Move(double dx, double dy) override
        {
            m_topLeft.Move(dx, dy);
        }

        [[nodiscard]] std::string GetType() const override
        {
            return m_type;
        }

        [[nodiscard]] std::string ToString() const override
        {
            std::ostringstream oss;
            oss << m_topLeft.GetX() << " " << m_topLeft.GetY() << " " << m_width << " " << m_height;
            return oss.str();
        }

    private:
        const std::string m_type = "rectangle";
        Point m_topLeft; 
        double m_width;
        double m_height;
    };
}

#endif //INC_4_RECTANGLEPAINTINGSTRATEGY_H