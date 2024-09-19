#ifndef MINUETDANCE_H
#define MINUETDANCE_H

#include "IDanceBehavior.h"
#include <iostream>

class MinuetDance : public IDanceBehavior
{
public:
    void Dance() const override
    {
        std::cout << "I'm dancing the Minuet!" << std::endl;
    }
};

#endif