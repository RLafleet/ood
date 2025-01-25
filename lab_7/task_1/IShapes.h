#ifndef ISHAPES_H
#define ISHAPES_H
#include <memory>

#include "IShape.h"

class IShapes
{
public:
	[[nodiscard]] virtual size_t GetShapesCount() const = 0;

	virtual void InsertShape(const std::shared_ptr<IShape>& shape, size_t position) = 0;

	virtual std::shared_ptr<IShape> GetShapeAtIndex(size_t index) = 0;

	virtual void RemoveShapeAtIndex(size_t index) = 0;

	virtual ~IShapes() = default;
};

#endif //ISHAPES_H_