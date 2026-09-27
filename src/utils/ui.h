#pragma once

#include "utils/color.h"

namespace ui
{
	const double TITLE_FONT_SIZE = 64.0;
	const double BUTTON_FONT_SIZE = 28.0;
	const double TEXT_FONT_SIZE = 22.0;

	struct Button
	{
		double x;
		double y;
		double width;
		double height;
		const char* text;
	};

	void init();

	void drawText(const char* text, double x, double y, double fontSize, const Color& color);
	void drawTextCentered(const char* text, double x, double y, double fontSize, const Color& color);
	void drawTextRight(const char* text, double x, double y, double fontSize, const Color& color);

	void drawButton(const Button& button, bool isSelected);

	int updateButtons(const Button buttons[], int count, int& selected);

	void drawOverlay();
}
