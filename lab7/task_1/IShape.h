#ifndef ISHAPE_H
#define ISHAPE_H

#include "Style/IStyle.h"
#include "CommonTypes.h"
#include "IDrawable.h"

class IShape : public IDrawable
{
public:
	[[nodiscard]] virtual RectD GetFrame() const = 0;

	virtual void SetFrame(const RectD& rect) = 0;

	virtual IStyle& GetOutlineStyle() = 0;

	[[nodiscard]] virtual IStyle& GetOutlineStyle() const = 0;

	virtual IStyle& GetFillStyle() = 0;

	[[nodiscard]] virtual IStyle& GetFillStyle() const = 0;

	~IShape() override = default;
};

#endif //ISHAPE_H