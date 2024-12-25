#ifndef GEOM_H
#define GEOM_H

struct Point
{
	int x = 0;
	int y = 0;
};

struct Size
{
	unsigned width = 0;
	unsigned height = 0;

	bool operator==(const Size& size) const
	{
		return size.width == width && size.height == height;
	}
};

// Точка передаётся в локальных координатах.
inline bool IsPointInSize(const Point p, const Size size) noexcept
{
	return static_cast<unsigned int>(p.x) < size.width && static_cast<unsigned int>(p.y) < size.height;
}

#endif //GEOM_H