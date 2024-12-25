#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include "CoW.h"
#include "Geom.h"
#include "Tile.h"

class Image
{
public:
	/**
	 * Конструирует изображение заданного размера. Если размеры не являются положительными,
	 * выбрасывает исключение std::out_of_range.
	 */
	explicit Image(const Size size, const uint32_t color = 0xFFFFFF)
		: m_size(size), m_tiles(size.height, std::vector(size.width, CoW(Tile(color))))
	{
	}

	// Возвращает размер изображения в пикселях.
	[[nodiscard]] Size GetSize() const noexcept
	{
		return m_size;
	}

	/**
	 * Возвращает «цвет» пикселя в указанных координатах.Если координаты выходят за пределы
	 * изображения, возвращает «пробел».
	 */
	[[nodiscard]] uint32_t GetPixel(const Point p) const noexcept
	{
		if (!IsPointInSize(p, m_size))
		{
			return 0xFFFFFF;
		}

		const int tileX = p.x / Tile::SIZE;
		const int tileY = p.y / Tile::SIZE;
		const int pixelX = p.x % Tile::SIZE;
		const int pixelY = p.y % Tile::SIZE;

		return m_tiles[tileY][tileX]->GetPixel({ pixelX, pixelY });
	}

	/**
	 * Задаёт «цвет» пикселя в указанных координатах. Если координаты выходят за пределы изображения
	 * действие игнорируется.
	 */
	void SetPixel(const Point p, const uint32_t color)
	{
		if (!IsPointInSize(p, m_size))
		{
			return;
		}

		const int tileX = p.x / Tile::SIZE;
		const int tileY = p.y / Tile::SIZE;
		const int pixelX = p.x % Tile::SIZE;
		const int pixelY = p.y % Tile::SIZE;

		m_tiles[tileY][tileX].Write([&](Tile& tile) {
			tile.SetPixel({ pixelX, pixelY }, color);
			});
	}

private:
	Size m_size;
	std::vector<std::vector<CoW<Tile> > > m_tiles;
};

#endif //IMAGE_H