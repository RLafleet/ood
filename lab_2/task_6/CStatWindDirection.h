#ifndef CSTATWINDDIRECTION_H
#define CSTATWINDDIRECTION_H

#include <cmath>
#include <limits>
#include <corecrt_math_defines.h>

class CStatWindDirection
{
public:
    void AddValue(double windDirection)
    {
        double angleRad = windDirection * M_PI / 180.0;
        m_sumSin += std::sin(angleRad);
        m_sumCos += std::cos(angleRad);
        ++m_count;
    }

    [[nodiscard]] double GetAverage() const
    {
        if (m_count == 0)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        double avgAngleRad = std::atan2(m_sumSin / m_count, m_sumCos / m_count);
        double avgAngleDeg = avgAngleRad * 180.0 / M_PI;

        if (avgAngleDeg < 0)
        {
            avgAngleDeg += 360.0;
        }
        if (avgAngleDeg >= 360.0)
        {
            avgAngleDeg -= 360.0; 
        }

        return avgAngleDeg;
    }

private:
    double m_sumSin = 0.0;
    double m_sumCos = 0.0;
    unsigned m_count = 0;
};

#endif // CSTATWINDDIRECTION_H
