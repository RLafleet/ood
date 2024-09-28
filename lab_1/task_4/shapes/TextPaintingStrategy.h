#ifndef INC_4_TEXT_H
#define INC_4_TEXT_H

#include <iostream>
#include <string>
#include <utility>
#include "IDrawable.h"

namespace shapes
{
    class TextPaintingStrategy : public IDrawable
    {
    public:
        TextPaintingStrategy(
            double leftTopX,
            double leftTopY,
            double size,
            std::string  text
        ) :
            m_leftTopX(leftTopX), m_leftTopY(leftTopY), m_size(size), m_text(std::move(text)) {}

        void Draw(gfx::ICanvas& canvas, uint32_t color) const override
        {
            canvas.SetColor(color);
            canvas.DrawText(m_leftTopX, m_leftTopY, m_size, m_text);
        }

        void Move(double dx, double dy) override
        {
            m_leftTopX += dx;
            m_leftTopY += dy;
        }

        [[nodiscard]] std::string GetType() const override
        {
            return m_type;
        }

        [[nodiscard]] std::string ToString() const override
        {
            std::ostringstream oss;
            oss << m_leftTopX << " " << m_leftTopY << " " << m_size << " " << m_text;
            return oss.str();
        }

    private:
        const std::string m_type = "text";
        double m_leftTopX;
        double m_leftTopY;
        double m_size;
        std::string m_text;
    };
}

#endif //INC_4_TEXT_H