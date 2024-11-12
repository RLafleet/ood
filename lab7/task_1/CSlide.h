#ifndef CSLIDE_H
#define CSLIDE_H

#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "ISlide.h"
#include "IShape.h"
#include "IGroupShape.h"
#include "GroupShape.h"

class CSlide final : public ISlide
{
public:
	CSlide(const double width, const double height)
		: m_width(width)
		, m_height(height)
	{
	}

	[[nodiscard]] double GetWidth() const override
	{
		return m_width;
	}

	[[nodiscard]] double GetHeight() const override
	{
		return m_height;
	}

	[[nodiscard]] size_t GetShapesCount() const override
	{
		return m_shapes.size();
	}

	[[nodiscard]] IShapes& GetShapes() const override
	{
		return *const_cast<CSlide*>(this);
	}

	std::shared_ptr<IShape> GetShapeAtIndex(const size_t index) override
	{
		if (index >= m_shapes.size())
		{
			throw std::out_of_range("Index is out of range");
		}
		return m_shapes[index];
	}

	void RemoveShapeAtIndex(const size_t index) override
	{
		if (index >= m_shapes.size())
		{
			throw std::out_of_range("Index is out of range");
		}
		m_shapes.erase(m_shapes.begin() + static_cast<std::vector<int>::difference_type>(index));
	}

	void GroupShapesAtIndexex(std::vector<size_t> indexes) override
	{
		std::vector<std::shared_ptr<IShape> > groupShapes;
		std::ranges::sort(indexes, std::greater<>());

		for (const size_t index : indexes)
		{
			if (index >= m_shapes.size())
			{
				throw std::out_of_range("Index is out of range");
			}
			groupShapes.push_back(m_shapes[index]);
			m_shapes.erase(m_shapes.begin() + static_cast<std::vector<int>::difference_type>(index));
		}

		const auto group = std::make_shared<GroupShape>();
		for (auto& shape : groupShapes)
		{
			group->InsertShape(shape, group->GetShapesCount());
		}
		m_shapes.push_back(group);
	}

	void InsertShape(const std::shared_ptr<IShape>& shape, size_t position) override
	{
		if (position > m_shapes.size())
		{
			position = m_shapes.size();
		}
		m_shapes.insert(m_shapes.begin() + static_cast<std::vector<int>::difference_type>(position), shape);
	}

	void Draw(ICanvas& canvas) const override
	{
		for (const auto& shape : m_shapes)
		{
			shape->Draw(canvas);
		}
	}

private:
	double m_width;
	double m_height;
	std::vector<std::shared_ptr<IShape> > m_shapes;
};

#endif //CSLIDE_H