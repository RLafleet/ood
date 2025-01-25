#ifndef PNGCANVAS_H
#define PNGCANVAS_H
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "ICanvas.h"
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include "corecrt_math_defines.h"
#include <algorithm>

class Canvas : public ICanvas
{
public:
	Canvas(const unsigned width, const unsigned height)
		: m_filling(false), m_fillingColor(0), m_width(width), m_height(height)
	{
		m_pixels.resize(width * height * 4, 255);
	}

	void BeginFill(RGBAColor color) override
	{
		m_filling = true;
		m_fillingColor = color;
	}

	void EndFill() override
	{
		m_filling = false;
	}

	void DrawEllipse(const RectD frame) override
	{
		const double cx = frame.left + frame.width / 2;
		const double cy = frame.top + frame.height / 2;
		const double rx = frame.width / 2;
		const double ry = frame.height / 2;

		if (m_filling)
		{
			FillEllipse(cx, cy, rx, ry);
		}

		constexpr int segments = 100;
		constexpr double angleStep = 2 * M_PI / segments;

		double prevX = cx + rx;
		double prevY = cy;

		for (int i = 1; i <= segments; ++i)
		{
			const double angle = i * angleStep;
			const double newX = cx + rx * cos(angle);
			const double newY = cy + ry * sin(angle);
			DrawLine(prevX, prevY, newX, newY);
			prevX = newX;
			prevY = newY;
		}
	}

	void DrawPolygon(std::vector<PointD> points) override
	{
		if (points.empty())
		{
			return;
		}

		if (m_filling)
		{
			FillPolygon(points);
		}

		for (size_t i = 0; i < points.size(); ++i)
		{
			const auto& start = points[i];
			const auto& end = points[(i + 1) % points.size()];
			DrawLine(start.x, start.y, end.x, end.y);
		}
	}

	void SetLineColor(std::optional<RGBAColor> color) override
	{
		m_lineColor = color;
	}


	void SaveToFile(const std::string& filename) const
	{
		stbi_write_png(
			filename.c_str(),
			static_cast<int>(m_width),
			static_cast<int>(m_height),
			4, m_pixels.data(),
			static_cast<int>(m_width) * 4
		);
	};

private:
	bool m_filling;
	RGBAColor m_fillingColor;
	std::optional<RGBAColor> m_lineColor;
	unsigned m_width;
	unsigned m_height;
	std::vector<uint8_t> m_pixels;

	void PutPixel(const int x, const int y)
	{
		if (!m_lineColor.has_value())
		{
			return;
		}

		const auto color = m_lineColor.value();

		if (x >= 0 && static_cast<size_t>(x) < m_width && y >= 0 && static_cast<size_t>(y) < m_height)
		{
			const auto index = 4 * (y * m_width + x);
			m_pixels[index] = static_cast<uint8_t>((color >> 24) & 0xFF);
			m_pixels[index + 1] = static_cast<uint8_t>((color >> 16) & 0xFF);
			m_pixels[index + 2] = static_cast<uint8_t>((color >> 8) & 0xFF);
			m_pixels[index + 3] = static_cast<uint8_t>(color & 0xFF);
		}
	}

	void DrawLine(const double x1, const double y1, const double x2, const double y2)
	{
		int ix1 = static_cast<int>(std::round(x1));
		int iy1 = static_cast<int>(std::round(y1));
		const int ix2 = static_cast<int>(std::round(x2));
		const int iy2 = static_cast<int>(std::round(y2));

		int dx = std::abs(ix2 - ix1), dy = std::abs(iy2 - iy1);
		const int sx = (ix1 < ix2) ? 1 : -1;
		const int sy = (iy1 < iy2) ? 1 : -1;
		int err = dx - dy;

		while (true)
		{
			PutPixel(ix1, iy1);
			if (ix1 == ix2 && iy1 == iy2)
			{
				break;
			}
			int e2 = 2 * err;
			if (e2 > -dy)
			{
				err -= dy;
				ix1 += sx;
			}
			if (e2 < dx)
			{
				err += dx;
				iy1 += sy;
			}
		}
	}

	void FillEllipse(const double cx, const double cy, const double rx, const double ry)
	{
		for (int y = static_cast<int>(cy - ry); y <= static_cast<int>(cy + ry); ++y)
		{
			for (int x = static_cast<int>(cx - rx); x <= static_cast<int>(cx + rx); ++x)
			{
				double dx = (x - cx) / rx;
				double dy = (y - cy) / ry;
				if (dx * dx + dy * dy <= 1)
				{
					const int index = 4 * (y * m_width + x);
					m_pixels[index] = (m_fillingColor >> 24) & 0xFF;
					m_pixels[index + 1] = (m_fillingColor >> 16) & 0xFF;
					m_pixels[index + 2] = (m_fillingColor >> 8) & 0xFF;
					m_pixels[index + 3] = m_fillingColor & 0xFF;
				}
			}
		}
	}

	void FillPolygon(const std::vector<PointD>& points)
	{
		double minY = points[0].y, maxY = points[0].y;
		for (const auto& p : points)
		{
			minY = std::min(minY, p.y);
			maxY = std::max(maxY, p.y);
		}

		for (int y = static_cast<int>(minY); y <= static_cast<int>(maxY); ++y)
		{
			std::vector<int> intersections;
			for (size_t i = 0; i < points.size(); ++i)
			{
				const auto& p1 = points[i];
				const auto& p2 = points[(i + 1) % points.size()];
				if ((p1.y <= y && p2.y > y) || (p2.y <= y && p1.y > y))
				{
					double x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
					intersections.push_back(static_cast<int>(x));
				}
			}

			std::sort(intersections.begin(), intersections.end());
			for (size_t i = 0; i < intersections.size(); i += 2)
			{
				for (int x = intersections[i]; x < intersections[i + 1]; ++x)
				{
					const int index = 4 * (y * m_width + x);
					m_pixels[index] = (m_fillingColor >> 24) & 0xFF;
					m_pixels[index + 1] = (m_fillingColor >> 16) & 0xFF;
					m_pixels[index + 2] = (m_fillingColor >> 8) & 0xFF;
					m_pixels[index + 3] = m_fillingColor & 0xFF;
				}
			}
		}
	}
};

#endif // PNGCANVAS_H_