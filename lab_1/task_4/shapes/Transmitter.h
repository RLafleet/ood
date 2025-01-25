#ifndef TRASMITTER_H
#define TRASMITTER_H

#include <string>
#include "FiguresType.h"

class Transmitter
{
public:
    static std::string ConvertShapeTypeToString(FiguresType shapeType)
    {
        switch (shapeType)
        {
        case FiguresType::RECTANGLE:
            return "rectangle";
        case FiguresType::CIRCLE:
            return "circle";
        case FiguresType::TRIANGLE:
            return "triangle";
        case FiguresType::LINE:
            return "line";
        case FiguresType::TEXT:
            return "text";
        default:
            return "none";
        }
    }
};


#endif //TRASMITTER_H