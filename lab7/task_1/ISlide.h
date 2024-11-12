#ifndef ISLIDE_H
#define ISLIDE_H

#include "IDrawable.h"
#include "IShapes.h"

class ISlide : public IDrawable, public IShapes
{
public:
	[[nodiscard]] virtual double GetWidth() const = 0;

	[[nodiscard]] virtual double GetHeight() const = 0;

	[[nodiscard]] virtual IShapes& GetShapes() const = 0;

	virtual void GroupShapesAtIndexex(std::vector<size_t> indexes) = 0;

	void Draw(ICanvas& canvas) const override = 0;

	~ISlide() override = default;
};

#endif //ISLIDE_H