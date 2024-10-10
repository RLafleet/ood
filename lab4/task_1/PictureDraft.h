#ifndef PICTUREDRAFT_H
#define PICTUREDRAFT_H

#include <vector>
#include <memory>
#include "Shape.h"

class PictureDraft
{
public:
    typedef std::vector<std::unique_ptr<Shape>> Storage;

    void AddShape(std::unique_ptr<Shape>&& shape)
    {
        m_shapes.push_back(std::move(shape));
    }

    Storage::iterator begin() {
        return m_shapes.begin();
    }

    Storage::iterator end() {
        return m_shapes.end();
    }

    [[nodiscard]] Storage::const_iterator begin() const {
        return m_shapes.begin();
    }

    [[nodiscard]] Storage::const_iterator end() const {
        return m_shapes.end();
    }

private:
    Storage m_shapes;
};
#endif //PICTUREDRAFT_H