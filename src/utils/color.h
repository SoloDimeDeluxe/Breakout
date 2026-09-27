#pragma once

struct Color
{
	double r;
	double g;
	double b;
	double a;
};

namespace colors
{
	const Color WHITE = { 1.0, 1.0, 1.0, 1.0 };
	const Color GRAY = { 0.6, 0.6, 0.65, 1.0 };
	const Color DARK_GRAY = { 0.2, 0.2, 0.28, 1.0 };
	const Color YELLOW = { 1.0, 0.85, 0.2, 1.0 };
	const Color SHADOW = { 0.0, 0.0, 0.0, 0.6 };

	void use(const Color& color);
}
