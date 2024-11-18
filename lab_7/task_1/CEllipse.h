#ifndef CELLIPSE_H
#define CELLIPSE_H

#include "CShape.h"
#include "ICanvas.h"

class CEllipse : public CShape
{
public:
	CEllipse(
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

		canvas.DrawEllipse(GetFrame());

		canvas.EndFill();
	}
};

#endif //CELLIPSE_H_