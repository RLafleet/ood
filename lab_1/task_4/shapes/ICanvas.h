#ifndef ICANVAS_H
#define ICANVAS_H

#include <string>
#include <cstdint>

namespace gfx
{
    class ICanvas
    {
    public:

        virtual void SetColor(uint32_t color) = 0;

        virtual void MoveTo(double x, double y) = 0;

        virtual void LineTo(double x, double y) = 0;

        virtual void DrawEllipse(double cx, double cy, double rx, double ry) = 0;
        virtual void DrawText(double left, double top, double fontSize, const std::string& text) = 0;

        virtual ~ICanvas() = default;
    };
}

#endif //INC_1_1_ICANVAS_H