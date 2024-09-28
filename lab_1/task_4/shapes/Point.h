#ifndef INC_4_POINT_H
#define INC_4_POINT_H

namespace shapes
{
    class Point
    {
    public:
        Point(double x = 0.0, double y = 0.0) : m_x(x), m_y(y) {}

        double GetX() const { return m_x; }
        double GetY() const { return m_y; }

        void Move(double dx, double dy)
        {
            m_x += dx;
            m_y += dy;
        }

    private:
        double m_x;
        double m_y;
    };
}

#endif //INC_4_POINT_H