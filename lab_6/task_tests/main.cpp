﻿#define CATCH_CONFIG_MAIN
#include "../../Catch2/catch.hpp"
#include <sstream>
#include <vector>

#include "../tasks/graphics_lib.h"
#include "../tasks/modern_graphics_lib.h"
#include "../tasks/ModernGraphicsAdapter.h"
#include "../tasks/ModernGraphicsClassAdapter.h"

namespace mgl = modern_graphics_lib;
namespace gl = graphics_lib;

mgl::CRGBAColor ConvertToColor(const uint32_t colorValue)
{
    constexpr float colorScale = 1.0f / 255.0f;
    const auto red = static_cast<float>((colorValue >> 16) & 0xFF) * colorScale;
    const auto green = static_cast<float>((colorValue >> 8) & 0xFF) * colorScale;
    const auto blue = static_cast<float>(colorValue & 0xFF) * colorScale;
    const auto alpha = static_cast<float>((colorValue >> 24) & 0xFF) * colorScale;

    return { red, green, blue, alpha };
}

std::string DrawWithoutAdapter(const std::vector<mgl::CPoint>& points, const uint32_t color)
{
    std::stringstream strm;
    mgl::CModernGraphicsRenderer renderer(strm);
    const auto rgbaColor = ConvertToColor(color);

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

void DrawWithAdapter(mgl::CModernGraphicsRenderer& renderer, graphics_lib::ICanvas& adapter,
    const std::vector<mgl::CPoint>& points, const uint32_t color)
{
    adapter.SetColor(color);
    renderer.BeginDraw();

    const auto first = points.begin();
    adapter.MoveTo(first->x, first->y);

    for (auto it = first + 1; it != points.end(); ++it)
    {
        adapter.LineTo(it->x, it->y);
    }

    adapter.LineTo(first->x, first->y);
    renderer.EndDraw();
}

std::string DrawWithObjectAdapter(const std::vector<mgl::CPoint>& points, const uint32_t color)
{
    std::stringstream strm;
    mgl::CModernGraphicsRenderer renderer(strm);
    app::ModernGraphicsAdapter adapter(renderer);

    DrawWithAdapter(renderer, adapter, points, color);

    return strm.str();
}

std::string DrawShapeWithClassAdapter(const std::vector<mgl::CPoint>& points, const uint32_t color)
{
    std::stringstream strm;
    app::ModernGraphicsClassAdapter adapter(strm);

    DrawWithAdapter(adapter, adapter, points, color);

    return strm.str();
}

TEST_CASE("TriangleTest - DrawWithObjectAdapter")
{
    const auto triangle =
    {
        mgl::CPoint(1, 2),
        mgl::CPoint(3, 4),
        mgl::CPoint(5, 6),
    };

    constexpr uint32_t color = 0xFFFFFFFF;

    const auto originalShape = DrawWithoutAdapter(triangle, color);
    const auto withAdapterShape = DrawWithObjectAdapter(triangle, color);

    REQUIRE(originalShape == withAdapterShape);
}

TEST_CASE("TriangleTest - DrawWithClassAdapter")
{
    const auto triangle =
    {
        mgl::CPoint(1, 2),
        mgl::CPoint(3, 4),
        mgl::CPoint(5, 6),
    };

    constexpr uint32_t color = 0xFFAAAAFF;

    const auto originalShape = DrawWithoutAdapter(triangle, color);
    const auto withAdapterShape = DrawShapeWithClassAdapter(triangle, color);

    REQUIRE(originalShape == withAdapterShape);
}