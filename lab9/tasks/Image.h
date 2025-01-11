#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include "CoW.h"
#include "Geom.h"
#include "Tile.h"

class Image
{
public:
	explicit Image(const Size size, const uint32_t color = 0xFFFFFF)
		: m_size(size), m_tiles(size.height, std::vector(size.width, CoW(Tile(color))))
	{
	}

	[[nodiscard]] Size GetSize() const noexcept
	{
		return m_size;
	}

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