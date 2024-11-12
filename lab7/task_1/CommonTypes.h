#ifndef COMMONTYPES_H
#define COMMONTYPES_H
#include <cstdint>

template<typename T>
struct Rect
{
	T left;
	T top;
	T width;
	T height;
};

template<typename T>
struct Point
{
	T x;
	T y;
};

typedef Point<double> PointD;
typedef Rect<double> RectD;
typedef uint32_t RGBAColor;

#endif //COMMONTYPES_H_