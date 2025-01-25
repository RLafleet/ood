#include "Picture.h"


// поместить в namespace shapes
void shapes::Picture::AddShape(const std::string& id, std::unique_ptr<Shape> shape)
{
    if (m_shapes.contains(id))
    {
        throw std::invalid_argument("Shape with given Id already exists");
    }
    m_shapes.emplace(id, std::move(shape));
    m_shapeIds.push_back(id); // может выбросить исключение
}

void shapes::Picture::MoveShape(const std::string &id, double dx, double dy)
{
    if (!m_shapes.contains(id))
    {
        throw std::invalid_argument("Shape with given Id not exists");
    }

    auto& shape = m_shapes.at(id);
    shape->Move(dx, dy);
}

void shapes::Picture::DeleteShape(const std::string &id)
{
    if (!m_shapes.contains(id))
    {
        throw std::invalid_argument("Shape with given Id not exists");
    }

    m_shapes.erase(id);
    std::erase(m_shapeIds, id);
}

void shapes::Picture::ChangeColor(const std::string &id, uint32_t color)
{
    if (!m_shapes.contains(id))
    {
        throw std::invalid_argument("Shape with given Id not exists");
    }

    auto& shape = m_shapes.at(id);
    shape->SetColor(color);
}

void shapes::Picture::MovePicture(double dx, double dy)
{
    for (const auto& pair : m_shapes)
    {
        pair.second->Move(dx, dy);
    }
}

void shapes::Picture::ChangeShape(const std::string &id, std::unique_ptr<IDrawable> newGeometryTypeStrategy)
{
    if (!m_shapes.contains(id))
    {
        throw std::invalid_argument("Shape with given Id not exists");
    }

    m_shapes.at(id)->SetDrawingStrategy(std::move(newGeometryTypeStrategy));
}

void shapes::Picture::DrawShape(const std::string &id, gfx::ICanvas &canvas)
{
    if (!m_shapes.contains(id))
    {
        throw std::invalid_argument("Shape with given Id not exists");
    }

    m_shapes.at(id)->Draw(canvas);
}

void shapes::Picture::DrawPicture(gfx::ICanvas &canvas)
{
    for (const auto& id : m_shapeIds) {
        m_shapes.at(id)->Draw(canvas);
    }
}