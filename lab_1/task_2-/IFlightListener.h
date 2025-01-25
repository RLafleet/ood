#ifndef IFLIGHTLISTENER_H
#define IFLIGHTLISTENER_H

class IFlightListener
{
public:
    virtual void OnFlightPerformed() = 0;
    virtual ~IFlightListener() = default;
};

#endif
