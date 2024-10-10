#pragma once
#ifndef CDESIGNER_H
#define CDESIGNER_H

#include "CPictureDraft.h"
#include "IDesigner.h"
#include "IShapeFactory.h"

class CDesigner : public IDesigner
{
public:
    explicit CDesigner(IShapeFactory& factory)
        : m_factory(factory) {}

    CPictureDraft CreateDraft(std::istream& inputData) override
    {
        CPictureDraft draft;
        std::string line;
        while (getline(inputData, line))
        {
            std::cout << line << std::endl;
            draft.AddShape(std::move(m_factory.CreateShape(line)));
        }
        return draft;
    }

private:
    IShapeFactory& m_factory;
};

#endif //CDESIGNER_H