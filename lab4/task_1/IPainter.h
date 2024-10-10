#ifndef IPAINTER_H
#define IPAINTER_H

#include "CPictureDraft.h"
#include "ICanvas.h"

class IPainter
{
public:
    virtual void DrawPicture(const CPictureDraft& draft, gfx::ICanvas& canvas) = 0;

    virtual ~IPainter() = default;
};

#endif //IPAINTER_H