#ifndef CSTAT_H
#define CSTAT_H

#include <iostream>
#include <limits>

class CStat
{
public:
    void AddValue(const double value)
    {
        if (m_minValue > value)
        {
            m_minValue = value;
        }
        if (m_maxValue < value)
        {
            m_maxValue = value;
        }
        m_accValue += value;
        ++m_countAcc;
    }

    [[nodiscard]] double GetAverage() const
    {
        return m_accValue / m_countAcc;
    }

    [[nodiscard]] double GetMin() const
    {
        return m_minValue;
    }

    [[nodiscard]] double GetMax() const
    {
        return m_maxValue;
    }

private:
    double m_minValue = std::numeric_limits<double>::infinity();
    double m_maxValue = -std::numeric_limits<double>::infinity();
    double m_accValue = 0;
    unsigned m_countAcc = 0;
};

#endif //CSTAT_H