#ifndef SVG_CANVAS_H
#define SVG_CANVAS_H

#include <string>
#include "ICanvas.h"

namespace gfx
{
    struct Color {
        uint8_t r, g, b, a;
    };

    Color StringToColor(uint32_t color);

    class SVGCanvas : public ICanvas
    {
    public:
        SVGCanvas(int w, int h);
        ~SVGCanvas() override;

        void SetColor(uint32_t color) override;
        void MoveTo(double x, double y) override;
        void DrawLine(double fromX, double fromY, double toX, double toY) override;
        void DrawEllipse(double cx, double cy, double rx, double ry) override;
        void DrawText(double left, double top, double fontSize, const std::string& text) override;
        void SaveToFile(const std::string& filename);

    private:
        //m_
        int width, height;
        Color currentColor;
        double currentX, currentY;
        std::string svgContent;
    };
}

#endif //SVG_CANVAS_H
