#ifndef IDRAWABLE_H
#define IDRAWABLE_H
#include "ICanvas.h"

class IDrawable
{
public:
	virtual void Draw(ICanvas& canvas) const = 0;

	virtual ~IDrawable() = default;
};

#endif //IDRAWABLE_H_