#ifndef ISHAPEFACTORY_H
#define ISHAPEFACTORY_H

#include "Shape.h"

class IShapeFactory
{
public:
    virtual std::unique_ptr<Shape> CreateShape(const std::string& description) = 0;

    virtual ~IShapeFactory() = default;
};

#endif //ISHAPEFACTORY_H