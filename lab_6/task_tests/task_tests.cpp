#define CATCH_CONFIG_MAIN
#include "../../Catch2/catch.hpp"
#include <sstream>
#include <vector>

#include "../tasks/graphics_lib.h"
#include "../tasks/modern_graphics_lib.h"
#include "../tasks/ModernGraphicsAdapter.h"
#include "../tasks/ModernGraphicsClassAdapter.h"

namespace mgl = modern_graphics_lib;
namespace gl = graphics_lib;

mgl::CRGBAColor ConvertColorHEXToRGBAColor(uint32_t colorValue)
{
    constexpr float colorScale = 1.0f / 255.0f;
    const float r = static_cast<float>((colorValue >> 16) & 0xFF) * colorScale;
    const float g = static_cast<float>((colorValue >> 8) & 0xFF) * colorScale;
    const float b = static_cast<float>(colorValue & 0xFF) * colorScale;
    const float a = static_cast<float>((colorValue >> 24) & 0xFF) * colorScale;

    mgl::CRGBAColor color = { r, g, b, a };
    return color;
}

std::string DrawWithoutAdapter(const std::vector<mgl::CPoint>& points, uint32_t color)
{
    std::stringstream strm;
    mgl::CModernGraphicsRenderer renderer(strm);
    auto rgbaColor = ConvertColorHEXToRGBAColor(color);

    const auto first = points.begin();
    renderer.BeginDraw();

    for (auto it = first; it != points.end(); ++it)
    {
        if (it + 1 != points.end())
        {
            renderer.DrawLine(*it, *(it + 1), rgbaColor);
        }
        else
        {
            renderer.DrawLine(*it, *first, rgbaColor);
        }
    }
    renderer.EndDraw();

    return strm.str();
}

void DrawWithAdapter(mgl::CModernGraphicsRenderer& renderer, graphics_lib::ICanvas& adapter, const std::vector<mgl::CPoint>& points, uint32_t color)
{
    renderer.BeginDraw();
    adapter.SetColor(color);

    auto first = points.begin();
    adapter.MoveTo((*first).x, (*first).y);

    for (auto it = first + 1; it != points.end(); ++it)
    {
        adapter.LineTo((*it).x, (*it).y);
    }

    adapter.LineTo((*first).x, (*first).y);

    renderer.EndDraw();
}

std::string DrawWithObjectAdapter(const std::vector<mgl::CPoint>& points, uint32_t color)
{
    std::stringstream strm;
    mgl::CModernGraphicsRenderer renderer(strm);
    app::ModernGraphicsAdapter adapter(renderer);

    DrawWithAdapter(renderer, adapter, points, color);
    return strm.str();
}

void DrawWithClassAdapter(app::ModernGraphicsClassAdapter& adapterP, graphics_lib::ICanvas& adapter,
    const std::vector<mgl::CPoint>& points, const uint32_t color)
{
    adapterP.BeginDraw();
    adapter.SetColor(color);

    const auto first = points.begin();
    adapter.MoveTo(first->x, first->y);

    for (auto it = first + 1; it != points.end(); ++it)
    {
        adapter.LineTo(it->x, it->y);
    }

    adapter.LineTo(first->x, first->y);

    adapterP.EndDraw();
}

std::string DrawShapeWithClassAdapter(const std::vector<mgl::CPoint>& points, const uint32_t color)
{
    std::stringstream strm;
    app::ModernGraphicsClassAdapter adapter(strm);

    DrawWithClassAdapter(adapter, adapter, points, color);

    return strm.str();
}

TEST_CASE("ObjectAdapterTest - InterfaceImplementation")
{
    std::stringstream strm;
    mgl::CModernGraphicsRenderer renderer(strm);
    app::ModernGraphicsAdapter adapter(renderer);

    graphics_lib::ICanvas* canvas = dynamic_cast<graphics_lib::ICanvas*>(&adapter);
    REQUIRE(canvas != nullptr);
}

TEST_CASE("ClassAdapterTest - InterfaceImplementation")
{
    std::stringstream strm;
    app::ModernGraphicsClassAdapter adapter(strm);

    graphics_lib::ICanvas* canvas = dynamic_cast<graphics_lib::ICanvas*>(&adapter);
    REQUIRE(canvas != nullptr);
}

TEST_CASE("ClassAdapterTest - DrawWithClassAdapter")
{
    auto triangle = {
        mgl::CPoint(10, 15),
        mgl::CPoint(100, 200),
        mgl::CPoint(150, 250),
    };

    uint32_t color = 0xFF5733FF;

    const auto originalShape = DrawWithoutAdapter(triangle, color);
    const auto withAdapterShape = DrawShapeWithClassAdapter(triangle, color);

    REQUIRE(originalShape == withAdapterShape);
}

TEST_CASE("ObjectAdapterTest - DrawWithObjectAdapter")
{
    auto triangle = {
        mgl::CPoint(10, 15),
        mgl::CPoint(100, 200),
        mgl::CPoint(150, 250),
    };

    uint32_t color = 0xFF5733FF;

    const auto originalShape = DrawWithoutAdapter(triangle, color);
    const auto withAdapterShape = DrawWithObjectAdapter(triangle, color);

    REQUIRE(originalShape == withAdapterShape);
}

TEST_CASE("ModernGraphicsAdapter - Initial Position Test with Color") {
    std::stringstream strm;
    mgl::CModernGraphicsRenderer renderer(strm);
    app::ModernGraphicsAdapter adapter(renderer);

    uint32_t color = 0x00000000;  
    auto rgbaColor = ConvertColorHEXToRGBAColor(color);

    SECTION("Verify initial MoveTo position and color") {
        renderer.BeginDraw();
        adapter.SetColor(color);
        adapter.MoveTo(10, 10);
        adapter.LineTo(20, 20);
        renderer.EndDraw();

        std::string expectedOutput =
            "<draw>\n"
            "  <line fromX=\"10\" fromY=\"10\" toX=\"20\" toY=\"20\">\n"
            "    <color r=\"" + std::to_string((int)rgbaColor.red) +
            "\" g=\"" + std::to_string((int)rgbaColor.green) +
            "\" b=\"" + std::to_string((int)rgbaColor.blue) +
            "\" a=\"" + std::to_string((int)rgbaColor.alpha) + "\" />\n"
            "  </line>\n"
            "</draw>\n";
        REQUIRE(strm.str() == expectedOutput);
    }
}

TEST_CASE("ModernGraphicsAdapter - MoveTo and LineTo Operations with Color") {
    std::stringstream strm;
    mgl::CModernGraphicsRenderer renderer(strm);
    app::ModernGraphicsAdapter adapter(renderer);

    uint32_t color = 0x00000000;  
    auto rgbaColor = ConvertColorHEXToRGBAColor(color);

    SECTION("LineTo draws line from current position with correct color") {
        renderer.BeginDraw();
        adapter.SetColor(color);
        adapter.MoveTo(10, 10);
        adapter.LineTo(20, 20);
        renderer.EndDraw();

        std::string expectedOutput =
            "<draw>\n"
            "  <line fromX=\"10\" fromY=\"10\" toX=\"20\" toY=\"20\">\n"
            "    <color r=\"" + std::to_string((int)rgbaColor.red) +
            "\" g=\"" + std::to_string((int)rgbaColor.green) +
            "\" b=\"" + std::to_string((int)rgbaColor.blue) +
            "\" a=\"" + std::to_string((int)rgbaColor.alpha) + "\" />\n"
            "  </line>\n"
            "</draw>\n";
        REQUIRE(strm.str() == expectedOutput);
    }
}

TEST_CASE("ModernGraphicsAdapter - Initial m_start Value with Color") {
    std::stringstream strm;
    mgl::CModernGraphicsRenderer renderer(strm);
    app::ModernGraphicsAdapter adapter(renderer);

    uint32_t color = 0x00000000;  
    auto rgbaColor = ConvertColorHEXToRGBAColor(color);

    SECTION("Verify initial m_start is (0,0) and uses correct color") {
        renderer.BeginDraw();
        adapter.SetColor(color);
        adapter.LineTo(15, 15);
        renderer.EndDraw();

        std::string expectedOutput =
            "<draw>\n"
            "  <line fromX=\"0\" fromY=\"0\" toX=\"15\" toY=\"15\">\n"
            "    <color r=\"" + std::to_string((int)rgbaColor.red) +
            "\" g=\"" + std::to_string((int)rgbaColor.green) +
            "\" b=\"" + std::to_string((int)rgbaColor.blue) +
            "\" a=\"" + std::to_string((int)rgbaColor.alpha) + "\" />\n"
            "  </line>\n"
            "</draw>\n";
        REQUIRE(strm.str() == expectedOutput);
    }
}

TEST_CASE("ModernGraphicsClassAdapter - MoveTo and LineTo Operations with Color") {
    std::stringstream strm;
    app::ModernGraphicsClassAdapter adapter(strm);

    uint32_t color = 0x00000000;  
    auto rgbaColor = ConvertColorHEXToRGBAColor(color);

    SECTION("LineTo draws line from current position with correct color") {
        adapter.BeginDraw();
        adapter.SetColor(color);
        adapter.MoveTo(10, 10);
        adapter.LineTo(20, 20);
        adapter.EndDraw();

        std::string expectedOutput =
            "<draw>\n"
            "  <line fromX=\"10\" fromY=\"10\" toX=\"20\" toY=\"20\">\n"
            "    <color r=\"" + std::to_string((int)rgbaColor.red) +
            "\" g=\"" + std::to_string((int)rgbaColor.green) +
            "\" b=\"" + std::to_string((int)rgbaColor.blue) +
            "\" a=\"" + std::to_string((int)rgbaColor.alpha) + "\" />\n"
            "  </line>\n"
            "</draw>\n";
        REQUIRE(strm.str() == expectedOutput);
    }
}

TEST_CASE("ModernGraphicsClassAdapter - Initial m_start Value with Color") {
    std::stringstream strm;
    app::ModernGraphicsClassAdapter adapter(strm);

    uint32_t color = 0x00000000;  
    auto rgbaColor = ConvertColorHEXToRGBAColor(color);

    SECTION("Verify initial m_start is (0,0) and uses correct color") {
        adapter.BeginDraw();
        adapter.SetColor(color);
        adapter.LineTo(15, 15);
        adapter.EndDraw();

        std::string expectedOutput =
            "<draw>\n"
            "  <line fromX=\"0\" fromY=\"0\" toX=\"15\" toY=\"15\">\n"
            "    <color r=\"" + std::to_string((int)rgbaColor.red) +
            "\" g=\"" + std::to_string((int)rgbaColor.green) +
            "\" b=\"" + std::to_string((int)rgbaColor.blue) +
            "\" a=\"" + std::to_string((int)rgbaColor.alpha) + "\" />\n"
            "  </line>\n"
            "</draw>\n";
        REQUIRE(strm.str() == expectedOutput);
    }
}