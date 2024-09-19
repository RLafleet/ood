#ifndef WALTZDANCE_H
#define WALTZDANCE_H

#include "IDanceBehavior.h"
#include <iostream>

class WaltzDance : public IDanceBehavior
{
public:
    void Dance() override
    {
        std::cout << "I'm dancing the Watlz!" << std::endl;
    }
};

#endif