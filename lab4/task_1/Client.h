#ifndef LAB4_CLIENT_H
#define LAB4_CLIENT_H

#include "ICanvas.h"
#include "IPainter.h"
#include "IDesigner.h"

// Клиент хранит интерфейс холста, он ему говорит 
class Client
{
public:
    explicit Client(IDesigner& designer)
        : m_designer(designer)
    {}

    void HandleCommand(std::istream& inputData, gfx::ICanvas& canvas, IPainter& painter)
    {
        CPictureDraft pictureDraft = m_designer.CreateDraft(inputData);

        painter.DrawPicture(pictureDraft, canvas);
    }

private:
    IDesigner& m_designer;
};

#endif //LAB4_CLIENT_H