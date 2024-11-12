#ifndef ICANVAS_H
#define ICANVAS_H

#include <optional>
#include <vector>
#include "CommonTypes.h"

class ICanvas
{
public:
	virtual void SetLineColor(std::optional<RGBAColor> color) = 0;

	virtual void BeginFill(RGBAColor color) = 0;

	virtual void EndFill() = 0;

	virtual void DrawEllipse(RectD frame) = 0;

	virtual void DrawPolygon(std::vector<PointD> points) = 0;

	virtual ~ICanvas() = 0;
};

inline ICanvas::~ICanvas() {}

#endif //ICANVAS_H_