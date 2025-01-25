#ifndef GROUPSHAPE_H
#define GROUPSHAPE_H

#include <limits>
#include "IGroupShape.h"
#include "CompositeStyle.h"

class GroupShape : public IGroupShape
{
public:
	[[nodiscard]] RectD GetFrame() const override
	{
		// не допускать создание пустых групп +
		if (m_shapes.empty())
		{
			throw std::logic_error("Group is empty. A group must contain at least one shape.");
		}

		double left = std::numeric_limits<double>::max();
		double top = std::numeric_limits<double>::max();
		double right = std::numeric_limits<double>::lowest();
		double bottom = std::numeric_limits<double>::lowest();

		for (const auto& shape : m_shapes)
		{
			RectD frame = shape->GetFrame();
			left = std::min(left, frame.left);
			top = std::min(top, frame.top);
			right = std::max(right, frame.left + frame.width);
			bottom = std::max(bottom, frame.top + frame.height);
		}

		return { left, top, right - left, bottom - top };
	}

	void SetFrame(const RectD& rect) override
	{
		auto [left, top, width, height] = GetFrame();
		const double scaleX = rect.width / width;
		const double scaleY = rect.height / height;

		for (const auto& shape : m_shapes)
		{
			const RectD shapeFrame = shape->GetFrame();
			const double newLeft = rect.left + (shapeFrame.left - left) * scaleX;
			const double newTop = rect.top + (shapeFrame.top - top) * scaleY;
			const double newWidth = shapeFrame.width * scaleX;
			const double newHeight = shapeFrame.height * scaleY;
			shape->SetFrame({ newLeft, newTop, newWidth, newHeight });
		}
	}

	IStyle& GetOutlineStyle() override
	{
		return *m_outlineStyleProxy;
	}

	[[nodiscard]] IStyle& GetOutlineStyle() const override
	{
		return *m_outlineStyleProxy;
	}

	IStyle& GetFillStyle() override
	{
		return *m_fillStyleProxy;
	}

	[[nodiscard]] IStyle& GetFillStyle() const override
	{
		return *m_fillStyleProxy;
	}

	[[nodiscard]] size_t GetShapesCount() const override
	{
		return m_shapes.size();
	}

	void InsertShape(const std::shared_ptr<IShape>& shape, size_t position) override
	{
		if (position > m_shapes.size())
		{
			position = m_shapes.size();
		}
		m_shapes.insert(m_shapes.begin() + static_cast<std::vector<int>::difference_type>(position), shape);
	}

	std::shared_ptr<IShape> GetShapeAtIndex(size_t index) override
	{
		if (index >= m_shapes.size())
		{
			throw std::out_of_range("Index is out of range");
		}
		return m_shapes[index];
	}

	void RemoveShapeAtIndex(size_t index) override
	{
		if (index >= m_shapes.size())
		{
			throw std::out_of_range("Index is out of range");
		}
		m_shapes.erase(m_shapes.begin() + static_cast<std::vector<int>::difference_type>(index));
	}

	void Draw(ICanvas& canvas) const override
	{
		for (const auto& shape : m_shapes)
		{
			shape->Draw(canvas);
		}
	}

private:
	std::vector<std::shared_ptr<IShape> > m_shapes;
	std::unique_ptr<CompositeStyle> m_outlineStyleProxy = std::make_unique<CompositeStyle>(true, m_shapes);
	std::unique_ptr<CompositeStyle> m_fillStyleProxy = std::make_unique<CompositeStyle>(false, m_shapes);
};

#endif //GROUPSHAPE_H_