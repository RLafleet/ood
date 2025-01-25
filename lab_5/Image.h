#ifndef IMAGE_H
#define IMAGE_H

#include "IImage.h"

class Image : public IImage
{
public:
    Image(const std::string& path, int width, int height)
        : m_path(path), m_width(width), m_height(height) {}

    [[nodiscard]] std::string GetPath()const override
    {
        return m_path;
    }

    [[nodiscard]] int GetWidth()const override
    {
        return m_width;
    }

    [[nodiscard]] int GetHeight()const override
    {
        return m_height;
    }

    void Resize(int width, int height) override
    {
        m_width = width;
        m_height = height;
    }

private:
    std::string m_path;
    int m_width;
    int m_height;
};

#endif //IMAGE_H