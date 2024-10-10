#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <string>
#include <cstdint>
#include <memory>
#include <utility>
#include "ICanvas.h"
#include "Color.h"

class CShape
{
public:
    explicit CShape(Color color) :
        m_color(color)
    {}

    virtual void Draw(gfx::ICanvas& canvas) const = 0;

    [[nodiscard]] Color GetColor() const
    {
        return m_color;
    }

    virtual ~CShape() = default;
private:
    Color m_color;
};

#endif //SHAPE_H