#ifndef LAB4_PICTUREDRAFT_H
#define LAB4_PICTUREDRAFT_H

#include <vector>
#include <memory>
#include "CShape.h"

class CPictureDraft
{
public:
    typedef std::vector<std::unique_ptr<CShape>> Storage;

    void AddShape(std::unique_ptr<CShape>&& shape)
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
#endif //LAB4_PICTUREDRAFT_H