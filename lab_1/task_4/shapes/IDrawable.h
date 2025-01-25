#ifndef IDRAWABLE_H
#define IDRAWABLE_H

#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
#include "ICanvas.h"

namespace shapes
{
    class IDrawable
    {
    public:
        virtual void Draw(gfx::ICanvas& canvas, uint32_t color) const = 0;

        virtual void Move(double dx, double dy) = 0;

        // выяснить и уметь объяснить зачем виртуальный деструктор
        virtual ~IDrawable() = default;

        [[nodiscard]] virtual std::string GetType() const = 0;

        [[nodiscard]] virtual std::string ToString() const = 0;
    };
}
#endif //IDRAWABLE_H