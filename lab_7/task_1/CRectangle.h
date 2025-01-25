#ifndef CRECTANGLE_H
#define CRECTANGLE_H

#include "CShape.h"
#include "ICanvas.h"

class CRectangle : public CShape
{
public:
	CRectangle(
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
			canvas.SetLineColor(std::nullopt);
		}

		const auto [left, top, width, height] = GetFrame();

		const std::vector<PointD> points = {
			{left, top},
			{left + width, top},
			{left + width, top + height},
			{left, top + height}
		};
		canvas.DrawPolygon(points);

		canvas.EndFill();
	}
};

#endif //CRECTANGLE_H_