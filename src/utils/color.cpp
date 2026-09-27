#include "utils/color.h"

#include "sl.h"

namespace colors
{
	void use(const Color& color)
	{
		slSetForeColor(color.r, color.g, color.b, color.a);
	}
}
