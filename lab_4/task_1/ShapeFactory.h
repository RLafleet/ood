#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <memory>
#include <algorithm>
#include <cctype>
#include "IShapeFactory.h"
#include "CTriangle.h"
#include "CEllipse.h"
#include "CRectangle.h"
#include "RegularPolygon.h"
#include <sstream> 

class ShapeFactory : public IShapeFactory
{
public:
    std::unique_ptr<Shape> CreateShape(const std::string& description) override
    {
        std::istringstream iss(description);
        
        std::string colorStr;
        iss >> colorStr;
        Color color = ShapeFactory::GetColor(colorStr);

        std::string shapeType;
        iss >> shapeType;

        if (shapeType == CTriangle::type)
        {
            return ShapeFactory::CreateTriangle(color, iss);
        }
        else if (shapeType == RegularPolygon::type)
        {
            return ShapeFactory::CreateRegularPolygon(color, iss);
        }
        else if (shapeType == CRectangle::type)
        {
            return ShapeFactory::CreateRectangle(color, iss);
        }
        else if (shapeType == CEllipse::type)
        {
            return ShapeFactory::CreateEllipse(color, iss);
        }
    
        throw std::invalid_argument("Unknown shape type");
    }

private:
    static Color GetColor(std::string& colorStr)
    {
        std::transform(colorStr.begin(), colorStr.end(), colorStr.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return convertToColor(colorStr);
    }

    static std::unique_ptr<CTriangle> CreateTriangle(Color color, std::istringstream& iss)
    {
        std::string x1;
        std::string x2;
        std::string x3;
        std::string y1;
        std::string y2;
        std::string y3;

        iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

        Point point1(std::stod(x1), std::stod(y1));
        Point point2(std::stod(x2), std::stod(y2));
        Point point3(std::stod(x3), std::stod(y3));

        return std::make_unique<CTriangle>(color, point1, point2, point3);
    }

    static std::unique_ptr<CEllipse> CreateEllipse(Color color, std::istringstream& iss)
    {
        std::string x1;
        std::string y1;
        std::string horizontalRadiusStr;
        std::string verticalRadiusStr;

        iss >> x1 >> y1 >> horizontalRadiusStr >> verticalRadiusStr;

        Point center(std::stod(x1), std::stod(y1));

        return std::make_unique<CEllipse>(
            color,
            center,
            std::stod(horizontalRadiusStr),
            std::stod(verticalRadiusStr)
        );
    }

    static std::unique_ptr<CRectangle> CreateRectangle(Color color, std::istringstream& iss)
    {
        std::string x1;
        std::string y1;
        std::string width;
        std::string height;

        iss >> x1 >> y1 >> width >> height;

        Point leftTop(std::stod(x1), std::stod(y1));

        return std::make_unique<CRectangle>(
            color,
            leftTop,
            std::stod(width),
            std::stod(height)
        );
    }

    static std::unique_ptr<RegularPolygon> CreateRegularPolygon(Color color, std::istringstream& iss)
    {
        std::string x1;
        std::string y1;
        std::string pointsCountStr;
        std::string radiusStr;

        iss >> x1 >> y1 >> pointsCountStr >> radiusStr;

        Point center(std::stod(x1), std::stod(y1));

        return std::make_unique<RegularPolygon>(
            color,
            center,
            std::stoi(pointsCountStr),
            std::stod(radiusStr)
        );
    }
};

#endif //SHAPEFACTORY_H