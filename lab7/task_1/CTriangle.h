#ifndef CTRIANGLE_H
#define CTRIANGLE_H

#include "CShape.h"
#include "ICanvas.h"

class CTriangle final : public CShape
{
public:
	CTriangle(
		const RectD& frame,
		std::unique_ptr<IStyle> outlineStyle,
		std::unique_ptr<IStyle> fillStyle
	) : CShape(frame, std::move(outlineStyle), std::move(fillStyle))
	{
	}

	void Draw(ICanvas& canvas) const override
	{
		const IStyle& outlineStyle = GetOutlineStyle();
		const IStyle& fillStyle = GetFillStyle();

		if (fillStyle.IsEnabled() && fillStyle.GetColor().has_value())
		{
			canvas.BeginFill(fillStyle.GetColor().value());
		}

		if (outlineStyle.IsEnabled() && outlineStyle.GetColor().has_value())
		{
			canvas.SetLineColor(outlineStyle.GetColor().value());
		}
		else
		{
			canvas.SetLineColor({});
		}

		const auto [left, top, width, height] = GetFrame();

		const std::vector<PointD> points = {
			{left + width / 2, top},
			{left, top + height},
			{left + width, top + height}
		};

		canvas.DrawPolygon(points);

		canvas.EndFill();
	}
};

#endif //CTRIANGLE_H