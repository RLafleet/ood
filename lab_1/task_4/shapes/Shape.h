#ifndef INC_4_SHAPE_H
#define INC_4_SHAPE_H

#include <iostream>
#include <string>
#include <cstdint>
#include <memory>
#include <utility>
#include "ICanvas.h"
#include "IDrawable.h"

namespace shapes
{
    class Shape
    {
    public:
        Shape(std::string id, uint32_t color, std::unique_ptr<IDrawable> geometryTypeStrategy) :
            m_id(std::move(id)), m_color(color), m_geometryTypeStrategy(std::move(geometryTypeStrategy))
        {}

        void Draw(gfx::ICanvas& canvas)
        {
            m_geometryTypeStrategy->Draw(canvas, m_color);
        }

        void Move(double dx, double dy)
        {
            m_geometryTypeStrategy->Move(dx, dy);
        }

        [[nodiscard]] std::string GetType() const
        {
            return m_geometryTypeStrategy->GetType();
        }

        [[nodiscard]] std::string GetId() const
        {
            return m_id;
        }

        void SetColor(uint32_t color)
        {
            m_color = color;
        }

        [[nodiscard]] uint32_t GetColor() const
        {
            return m_color;
        }

        [[nodiscard]] std::string GetStrategyToString() const
        {
            return m_geometryTypeStrategy->ToString();
        }

        void SetDrawingStrategy(std::unique_ptr<IDrawable> geometryTypeStrategy) {
            m_geometryTypeStrategy = std::move(geometryTypeStrategy);
        }

        ~Shape() = default;
    private:
        std::string m_id;
        uint32_t m_color;
        std::unique_ptr<IDrawable> m_geometryTypeStrategy;
    };
}

#endif //INC_4_SHAPE_H