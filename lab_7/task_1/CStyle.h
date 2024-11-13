#ifndef CSTYLE_H
#define CSTYLE_H
#include "IStyle.h"

class CStyle final : public IStyle
{
public:
	[[nodiscard]] std::optional<bool> IsEnabled() const override
	{
		return m_enabled;
	}

	void SetIsEnabled(const bool enabled) override
	{
		m_enabled = enabled;
	}

	[[nodiscard]] std::optional<RGBAColor> GetColor() const override
	{
		return m_color;
	}

	void SetColor(RGBAColor color) override
	{
		m_color = color;
	}

private:
	bool m_enabled = false;
	std::optional<RGBAColor> m_color;
};

#endif //CSTYLE_H_