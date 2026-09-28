#include "utils/ui.h"

#include "sl.h"

#include "game/config.h"
#include "utils/input.h"

namespace ui
{
	namespace
	{
		int font = -1;

		bool isMouseOver(const Button& button)
		{
			double mouseX = input::getMouseX();
			double mouseY = input::getMouseY();

			return mouseX >= button.x - button.width / 2.0 &&
				mouseX <= button.x + button.width / 2.0 &&
				mouseY >= button.y - button.height / 2.0 &&
				mouseY <= button.y + button.height / 2.0;
		}

		void prepareText(double fontSize, const Color& color, int align)
		{
			slSetFont(font, static_cast<int>(fontSize));
			slSetTextAlign(align);
			colors::use(color);
		}
	}

	void init()
	{
		font = slLoadFont(config::FONT_PATH);
		slSetFont(font, static_cast<int>(TEXT_FONT_SIZE));
	}

	void drawText(const char* text, double x, double y, double fontSize, const Color& color)
	{
		prepareText(fontSize, color, SL_ALIGN_LEFT);
		slText(x, y, text);
	}

	void drawTextCentered(const char* text, double x, double y, double fontSize, const Color& color)
	{
		prepareText(fontSize, color, SL_ALIGN_CENTER);
		slText(x, y, text);
	}

	void drawTextRight(const char* text, double x, double y, double fontSize, const Color& color)
	{
		prepareText(fontSize, color, SL_ALIGN_RIGHT);
		slText(x, y, text);
	}

	void drawButton(const Button& button, bool isSelected)
	{
		colors::use(isSelected ? colors::YELLOW : colors::DARK_GRAY);
		slRectangleFill(button.x, button.y, button.width, button.height);

		colors::use(colors::WHITE);
		slRectangleOutline(button.x, button.y, button.width, button.height);

		const Color& textColor = isSelected ? colors::DARK_GRAY : colors::WHITE;
		drawTextCentered(button.text, button.x, button.y - BUTTON_FONT_SIZE / 3.0, BUTTON_FONT_SIZE, textColor);
	}

	int updateButtons(const Button buttons[], int count, int& selected)
	{
		if (input::isKeyPressed(SL_KEY_UP) || input::isKeyPressed('W'))
		{
			selected = (selected - 1 + count) % count;
		}
		if (input::isKeyPressed(SL_KEY_DOWN) || input::isKeyPressed('S'))
		{
			selected = (selected + 1) % count;
		}
		if (input::isKeyPressed(SL_KEY_ENTER) || input::isKeyPressed(' '))
		{
			return selected;
		}

		for (int i = 0; i < count; i++)
		{
			if (isMouseOver(buttons[i]))
			{
				selected = i;

				if (input::isMouseLeftPressed())
				{
					return i;
				}
			}
		}

		return -1;
	}

	void drawMenuHint(double y)
	{
		drawTextCentered("Flechas / W S para elegir - ENTER o click para aceptar",
			config::SCREEN_WIDTH / 2.0, y, TEXT_FONT_SIZE - 4.0, colors::GRAY);
	}

	void drawOverlay()
	{
		colors::use(colors::SHADOW);
		slRectangleFill(config::SCREEN_WIDTH / 2.0, config::SCREEN_HEIGHT / 2.0,
			config::SCREEN_WIDTH, config::SCREEN_HEIGHT);
	}
}
