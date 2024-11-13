#ifndef PROXYSTYLE_H
#define PROXYSTYLE_H
#include <memory>
#include <vector>

#include "IStyle.h"
#include "IShape.h"

class ProxyStyle final : public IStyle
{
public:
	ProxyStyle(const bool isOutline, std::vector<std::shared_ptr<IShape> >& shapes)
		: m_isOutline(isOutline)
		, m_shapes(shapes)
	{
		if (!m_shapes.empty())
		{
			const IStyle& initialStyle = m_isOutline ? m_shapes[0]->GetOutlineStyle() : m_shapes[0]->GetFillStyle();
			m_enabled = initialStyle.IsEnabled();
			m_color = initialStyle.GetColor();
		}
	}

	[[nodiscard]] std::optional<bool> IsEnabled() const override
	{
		if (m_shapes.empty())
		{
			return std::nullopt;
		}

		const std::optional<bool> commonEnabled = m_isOutline
			? m_shapes[0]->GetOutlineStyle().IsEnabled()
			: m_shapes[0]->GetFillStyle().IsEnabled();
		for (const auto& shape : m_shapes)
		{
			const auto styleEnabled = m_isOutline
				? shape->GetOutlineStyle().IsEnabled()
				: shape->GetFillStyle().IsEnabled();
			if (commonEnabled != styleEnabled)
			{
				return std::nullopt;
			}
		}
		return commonEnabled;
	}

	void SetIsEnabled(bool enabled) override
	{
		m_enabled = enabled;
		for (const auto& shape : m_shapes)
		{
			if (m_isOutline)
			{
				shape->GetOutlineStyle().SetIsEnabled(enabled);
			}
			else
			{
				shape->GetFillStyle().SetIsEnabled(enabled);
			}
		}
	}

	[[nodiscard]] std::optional<RGBAColor> GetColor() const override
	{
		if (m_shapes.empty())
		{
			return std::nullopt;
		}

		std::optional<RGBAColor> commonColor = m_isOutline
			? m_shapes[0]->GetOutlineStyle().GetColor()
			: m_shapes[0]->GetFillStyle().GetColor();
		for (const auto& shape : m_shapes)
		{
			const auto styleColor =
				m_isOutline ? shape->GetOutlineStyle().GetColor() : shape->GetFillStyle().GetColor();
			if (commonColor != styleColor)
			{
				return std::nullopt;
			}
		}
		return commonColor;
	}

	void SetColor(RGBAColor color) override
	{
		m_color = color;
		for (const auto& shape : m_shapes)
		{
			if (m_isOutline)
			{
				shape->GetOutlineStyle().SetColor(color);
			}
			else
			{
				shape->GetFillStyle().SetColor(color);
			}
		}
	}

private:
	bool m_isOutline;
	std::vector<std::shared_ptr<IShape> >& m_shapes;
	std::optional<bool> m_enabled;
	std::optional<RGBAColor> m_color;
};

#endif //PROXYSTYLE_H_