#ifndef IQUACKSOUND_H
#define IQUACKSOUND_H

struct IQuackSound
{
	virtual ~IQuackSound() = default;

	virtual bool AllowQuacking(int flightAmount) const = 0;
};

#endif //IQUACKSOUND_H