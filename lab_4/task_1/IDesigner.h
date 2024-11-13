#ifndef IDESIGNER_H
#define IDESIGNER_H

#include "PictureDraft.h"

class IDesigner
{
public:
    virtual PictureDraft CreateDraft(std::istream& inputData) = 0;

    virtual ~IDesigner() = default;
};

#endif //IDESIGNER_H