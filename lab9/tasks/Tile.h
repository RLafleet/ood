#ifndef TILE_H
#define TILE_H

#include <array>
#include <cassert>
#include <cstdint>
#include "Geom.h"

class Tile
{
public:
	constexpr static int SIZE = 8;

	explicit Tile(const uint32_t color = 0xFFFFFF) noexcept
	{
		for (int y = 0; y < SIZE; ++y)
		{
			for (int x = 0; x < SIZE; ++x)
			{
				m_pixels[y][x] = color;
			}
		}

		assert(m_instanceCount >= 0);
		++m_instanceCount; 
	}

	Tile(const Tile& other)
	{
		m_pixels = other.m_pixels; 

		assert(m_instanceCount >= 0);
		++m_instanceCount; 
	}

	~Tile()
	{
		--m_instanceCount; 
		assert(m_instanceCount >= 0);
	}

	void SetPixel(const Point p, const uint32_t color) noexcept
	{
		if (IsPointInSize(p, { SIZE, SIZE }))
		{
			m_pixels[p.y][p.x] = color;
		}
	}

	[[nodiscard]] uint32_t GetPixel(const Point p) const noexcept
	{
		if (!IsPointInSize(p, { SIZE, SIZE }))
		{
			return 0xFFFFFF; 
		}

		return m_pixels[p.y][p.x]; 
	}

	static int GetInstanceCount() noexcept
	{
		return m_instanceCount;
	}

private:
	inline static int m_instanceCount = 0;

	std::array<std::array<uint32_t, SIZE>, SIZE> m_pixels{};
};

#endif //TILE_H