#ifndef SHAPETYPE_H
#define SHAPETYPE_H
#include <string>

enum class ShapeType
{
	ELLIPSE,
	RECTANGLE,
	TRIANGLE,
};

inline std::string ShapeTypeToString(const ShapeType shape)
{
	switch (shape)
	{
	case ShapeType::ELLIPSE:
		return "ellipse";
	case ShapeType::RECTANGLE:
		return "rectangle";
	case ShapeType::TRIANGLE:
		return "triangle";
	default:
		return "unknown";
	}
}

#endif //SHAPETYPE_H_