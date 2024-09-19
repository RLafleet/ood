#ifndef LAB01_SIMUDUCK_IDANCEBEHAVIOR_H
#define LAB01_SIMUDUCK_IDANCEBEHAVIOR_H

struct IDanceBehavior
{
	virtual ~IDanceBehavior() {};
	virtual void Dance() const = 0;
};

#endif
