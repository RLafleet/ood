#ifndef ISTYLE_H
#define ISTYLE_H
#include <optional>
#include "CommonTypes.h"

class IStyle
{
public:
	[[nodiscard]] virtual std::optional<bool> IsEnabled() const = 0;

	virtual void SetIsEnabled(bool enabled) = 0;

	[[nodiscard]] virtual std::optional<RGBAColor> GetColor() const = 0;

	virtual void SetColor(RGBAColor color) = 0;

	virtual ~IStyle() = default;
};

#endif //ISTYLE_H_