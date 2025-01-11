#ifndef DRAWER_H
#define DRAWER_H

#include <iostream>
#include <cassert>
#include "Image.h"
#include "Geom.h"

class Drawer
{
public:
	static void DrawLine(Image& image, const Point from, const Point to, const uint32_t color)
	{
		const int deltaX = std::abs(to.x - from.x);
		const int deltaY = std::abs(to.y - from.y);

		if (deltaY > deltaX)
		{
			DrawSteepLine(image, from, to, color);
		}
		else
		{
			DrawSlopeLine(image, from, to, color);
		}
	}

	static void DrawCircle(Image& image, const Point center, const int radius, const uint32_t color)
	{
		int x = 0;
		int y = radius;
		int d = 3 - 2 * radius;

		while (x <= y)
		{
			image.SetPixel({ center.x + x, center.y + y }, color);
			image.SetPixel({ center.x - x, center.y + y }, color);
			image.SetPixel({ center.x + x, center.y - y }, color);
			image.SetPixel({ center.x - x, center.y - y }, color);
			image.SetPixel({ center.x + y, center.y + x }, color);
			image.SetPixel({ center.x - y, center.y + x }, color);
			image.SetPixel({ center.x + y, center.y - x }, color);
			image.SetPixel({ center.x - y, center.y - x }, color);

			if (d < 0)
			{
				d = d + 4 * x + 6;
			}
			else
			{
				d = d + 4 * (x - y) + 10;
				--y;
			}
			++x;
		}
	}

	static void FillCircle(Image& image, const Point center, const int radius, const uint32_t color)
	{
		int x = 0;
		int y = radius;
		int d = 3 - 2 * radius;

		while (x <= y)
		{
			if (y != x)
			{
				DrawLine(image, { center.x - y, center.y + x }, { center.x + y, center.y + x }, 0xFF0000);
				if (x != 0)
				{
					DrawLine(image, { center.x - y, center.y - x }, { center.x + y, center.y - x }, 0xFF0000);
				}
			}

			DrawLine(image, { center.x - x, center.y + y }, { center.x + x, center.y + y }, 0x0000FF);
			if (y != 0)
			{
				DrawLine(image, { center.x - x, center.y - y }, { center.x + x, center.y - y }, 0x0000FF);
			}

			if (d < 0)
			{
				d = d + 4 * x + 6;
			}
			else
			{
				d = d + 4 * (x - y) + 10;
				--y;
			}
			++x;
		}
	}

private:
	static int Sign(const int value)
	{
		return (0 < value) - (value < 0);
	}

	static void DrawSteepLine(Image& image, Point from, Point to, const uint32_t color)
	{
		const int deltaX = std::abs(to.x - from.x);
		const int deltaY = std::abs(to.y - from.y);

		assert(deltaY >= deltaX);

		if (from.y > to.y)
		{
			std::swap(from, to);
		}

		const int stepX = Sign(to.x - from.x); 
		const int errorThreshold = deltaY + 1; 
		const int deltaErr = deltaX + 1; 

		int error = deltaErr / 2;

		for (Point p = from; p.y <= to.y; ++p.y)
		{
			image.SetPixel({ p.x, p.y }, color);
			assert((p.y != to.y) || (p.x == to.x));

			error += deltaErr;

			if (error >= errorThreshold)
			{
				p.x += stepX; 
				error -= errorThreshold;
			}
		}
	}

	static void DrawSlopeLine(Image& image, Point from, Point to, const uint32_t color)
	{
		const int deltaX = std::abs(to.x - from.x);
		const int deltaY = std::abs(to.y - from.y);

		assert(deltaX >= deltaY);

		if (from.x > to.x)
		{
			std::swap(from, to);
		}

		const int stepY = Sign(to.y - from.y);
		const int errorThreshold = deltaX + 1;
		const int deltaErr = deltaY + 1;

		int error = deltaErr / 2;

		for (Point p = from; p.x <= to.x; ++p.x)
		{
			image.SetPixel({ p.x, p.y }, color);
			assert((p.x != to.x) || (p.y == to.y));

			error += deltaErr;

			if (error >= errorThreshold)
			{
				p.y += stepY;
				error -= errorThreshold;
			}
		}
	}
};

#endif //DRAWER_H