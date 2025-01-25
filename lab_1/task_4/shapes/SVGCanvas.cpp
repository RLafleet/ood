#include "SVGCanvas.h"
#include <fstream>
#include <iostream>

namespace gfx
{
    Color StringToColor(uint32_t color)
    {
        Color result{};
        result.r = static_cast<uint8_t>((color >> 24) & 0xFF);
        result.g = static_cast<uint8_t>((color >> 16) & 0xFF);
        result.b = static_cast<uint8_t>((color >> 8) & 0xFF);
        result.a = static_cast<uint8_t>(color & 0xFF);
        return result;
    }

    SVGCanvas::SVGCanvas(int w, int h) : width(w), height(h), currentColor({ 255, 255, 255, 255 }), currentX(0), currentY(0)
    {
        svgContent += "<?xml version=\"1.0\" standalone=\"no\"?>\n";
        svgContent += "<svg width=\"" + std::to_string(width) + "\" height=\"" + std::to_string(height) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    }

    SVGCanvas::~SVGCanvas()
    {
        svgContent += "</svg>\n";
        SaveToFile("output.svg");
    }

    void SVGCanvas::SetColor(uint32_t color)
    {
        currentColor = StringToColor(color);
    }

    void SVGCanvas::MoveTo(double x, double y)
    {
        currentX = x;
        currentY = y;
    }

    void SVGCanvas::LineTo(double x, double y)
    {
        svgContent += "<line x1=\"" + std::to_string(currentX) + "\" y1=\"" + std::to_string(currentY)
            + "\" x2=\"" + std::to_string(x) + "\" y2=\"" + std::to_string(y)
            + "\" stroke=\"rgb(" + std::to_string(currentColor.r) + ","
            + std::to_string(currentColor.g) + ","
            + std::to_string(currentColor.b) + ")\" />\n";
        MoveTo(x, y);
    }

    void SVGCanvas::DrawEllipse(double cx, double cy, double rx, double ry)
    {
        svgContent += "<ellipse cx=\"" + std::to_string(cx) + "\" cy=\"" + std::to_string(cy)
            + "\" rx=\"" + std::to_string(rx) + "\" ry=\"" + std::to_string(ry)
            + "\" fill=\"none\" stroke=\"rgb(" + std::to_string(currentColor.r) + ","
            + std::to_string(currentColor.g) + ","
            + std::to_string(currentColor.b) + ")\" />\n";
    }

    void SVGCanvas::DrawText(double left, double top, double fontSize, const std::string& text)
    {
        svgContent += "<text x=\"" + std::to_string(left) + "\" y=\"" + std::to_string(top + fontSize)
            + "\" font-size=\"" + std::to_string(fontSize)
            + "\" fill=\"rgb(" + std::to_string(currentColor.r) + ","
            + std::to_string(currentColor.g) + ","
            + std::to_string(currentColor.b) + ")\" "
            + "font-family=\"Arial\">" + text + "</text>\n";
    }

    void SVGCanvas::SaveToFile(const std::string& filename)
    {
        std::ofstream file(filename);
        if (file.is_open())
        {
            file << svgContent;
            file.close();
        }
        else
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
        }
    }
}
