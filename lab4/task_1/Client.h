#ifndef CLIENT_H
#define CLIENT_H

#include "ICanvas.h"
#include "IPainter.h"
#include "IDesigner.h"

class Client
{
public:
    explicit Client(IDesigner& designer)
        : m_designer(designer)
    {}

    void HandleCommand(std::istream& inputData, gfx::ICanvas& canvas, IPainter& painter)
    {
        PictureDraft pictureDraft = m_designer.CreateDraft(inputData);

        painter.DrawPicture(pictureDraft, canvas);
    }

private:
    IDesigner& m_designer;
};

#endif //CLIENT_H