#include <iostream>
#include <string>
#include <sstream> 
#include "IDrawable.h"
#include "Point.h" 

namespace shapes
{
    class LinePaintingStrategy : public IDrawable
    {
    public:
        LinePaintingStrategy(const Point& start, const Point& end)
            : m_start(start), m_end(end) {}

        void Draw(gfx::ICanvas& canvas, uint32_t color) const override
        {
            canvas.SetColor(color);
            canvas.MoveTo(m_start.GetX(), m_start.GetY());
            canvas.LineTo(m_end.GetX(), m_end.GetY());
        }

        void Move(double dx, double dy) override
        {
            m_start.Move(dx, dy);
            m_end.Move(dx, dy);
        }

        [[nodiscard]] std::string GetType() const override
        {
            return m_type;
        }

        [[nodiscard]] std::string ToString() const override
        {
            std::ostringstream oss;
            oss << m_start.GetX() << " " << m_start.GetY() << " "
                << m_end.GetX() << " " << m_end.GetY();
            return oss.str();
        }

    private:
        const std::string m_type = "line";
        Point m_start; 
        Point m_end;   
    };
}
