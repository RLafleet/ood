#ifndef WALTZDANCE_H
#define WALTZDANCE_H

#include "IDanceBehavior.h"
#include <iostream>

class WaltzDance : public IDanceBehavior
{
public:
    void Dance() const override
    {
        std::cout << "I'm dancing the Watlz!" << std::endl;
    }
};

#endif