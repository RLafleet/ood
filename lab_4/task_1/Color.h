#ifndef COLOR_H
#define COLOR_H

#include <map>
#include <string>
#include <cstdint> 

enum class Color
{
    GREEN,
    RED,
    BLUE,
    YELLOW,
    PINK,
    BLACK,
};

const std::map<Color, uint32_t> colorsToHEX = {
    {Color::GREEN, 0xFF00FF00},
    {Color::RED, 0xFFFF0000},
    {Color::BLUE, 0xFF0000FF},
    {Color::YELLOW, 0xFFFFFF00},
    {Color::PINK, 0xFFFFC0CB},
    {Color::BLACK, 0xFF000000}
};

const std::map<std::string, Color> stringToColor = {
        {"green", Color::GREEN},
        {"red", Color::RED},
        {"blue", Color::BLUE},
        {"yellow", Color::YELLOW},
        {"pink", Color::PINK},
        {"black", Color::BLACK}
};

uint32_t convertColorToHEX(Color color)
{
    return colorsToHEX.at(color);
}

Color convertToColor(const std::string& colorStr)
{
    return stringToColor.at(colorStr);
}

#endif //COLOR_H