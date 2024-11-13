#ifndef INC_4_PICTURE_H
#define INC_4_PICTURE_H

#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <map>
#include "FiguresType.h"
#include "Transmitter.h"
#include "Shape.h"
#include "IDrawable.h"
#include "RectanglePaintingStrategy.h"
#include "CirclePaintingStrategy.h"
#include "TrianglePaintingStrategy.h"
#include "LinePaintingStrategy.h"
#include "TextPaintingStrategy.h"
#include "ICanvas.h"

namespace shapes
{
    class Picture
    {
    public: 
        void AddShape(const std::string& id, std::unique_ptr<Shape> shape);

        void MoveShape(const std::string& id, double dx, double dy);

        void MovePicture(double dx, double dy);

        void DeleteShape(const std::string& id);

        template <typename Callback>
        void List(Callback callback) {
            for (const auto& id : m_shapeIds) {
                auto& shape = m_shapes.at(id);
                callback(shape->GetType(), id, shape->GetColor(), shape->GetStrategyToString());
            }
        }

        void ChangeColor(const std::string& id, uint32_t color);

        void ChangeShape(const std::string& id, std::unique_ptr<IDrawable> newGeometryTypeStrategy);

        void DrawShape(const std::string& id, gfx::ICanvas& canvas);

        void DrawPicture(gfx::ICanvas& canvas);

    private:
        std::map<std::string, std::unique_ptr<shapes::Shape>> m_shapes = {};
        std::vector<std::string> m_shapeIds = {};
    };
}

#endif //INC_4_PICTURE_H