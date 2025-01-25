#ifndef IPARAGRAPH_H
#define IPARAGRAPH_H

#include <string>

class IParagraph
{
public:
    virtual std::string GetText()const = 0;
    virtual void SetText(const std::string& text) = 0;
    virtual ~IParagraph() = default;
};

#endif //IPARAGRAPH_H