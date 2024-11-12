#ifndef CSHAPE_H
#define CSHAPE_H

#include "IGroupShape.h"
#include "IShape.h"
#include "CommonTypes.h"
#include "ICanvas.h"

class CShape : public IShape
{
public:
	CShape(
		const RectD& frame,
		std::unique_ptr<IStyle> outlineStyle,
		std::unique_ptr<IStyle> fillStyle
	) : m_frame(frame),
		m_outlineStyle(std::move(outlineStyle)),
		m_fillStyle(std::move(fillStyle))
	{
	}

	[[nodiscard]] RectD GetFrame() const override
	{
		return m_frame;
	}

	void SetFrame(const RectD& rect) override
	{
		m_frame = rect;
	}

	IStyle& GetOutlineStyle() override
	{
		return *m_outlineStyle;
	}

	[[nodiscard]] IStyle& GetOutlineStyle() const override
	{
		return *m_outlineStyle;
	}

	IStyle& GetFillStyle() override
	{
		return *m_fillStyle;
	}

	[[nodiscard]] IStyle& GetFillStyle() const override
	{
		return *m_fillStyle;
	}

	void Draw(ICanvas& canvas) const override = 0;

	~CShape() override = default;

private:
	RectD m_frame;
	std::unique_ptr<IStyle> m_outlineStyle;
	std::unique_ptr<IStyle> m_fillStyle;
};

#endif //CSHAPE_H