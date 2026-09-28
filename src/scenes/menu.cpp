#include "scenes/menu.h"

#include "game/config.h"
#include "utils/ui.h"

namespace menu
{
	namespace
	{
		enum Option
		{
			OPTION_PLAY,
			OPTION_INSTRUCTIONS,
			OPTION_CREDITS,
			OPTION_EXIT,
			OPTION_COUNT
		};

		const double BUTTON_WIDTH = 260.0;
		const double BUTTON_HEIGHT = 56.0;
		const double CENTER_X = config::SCREEN_WIDTH / 2.0;

		const ui::Button BUTTONS[OPTION_COUNT] =
		{
			{ CENTER_X, 420.0, BUTTON_WIDTH, BUTTON_HEIGHT, "Jugar" },
			{ CENTER_X, 340.0, BUTTON_WIDTH, BUTTON_HEIGHT, "Como jugar" },
			{ CENTER_X, 260.0, BUTTON_WIDTH, BUTTON_HEIGHT, "Creditos" },
			{ CENTER_X, 180.0, BUTTON_WIDTH, BUTTON_HEIGHT, "Salir" }
		};

		int selectedOption = OPTION_PLAY;
	}

	void init()
	{
		selectedOption = OPTION_PLAY;
	}

	game::Scene update()
	{
		int chosen = ui::updateButtons(BUTTONS, OPTION_COUNT, selectedOption);

		switch (chosen)
		{
		case OPTION_PLAY:
			return game::Scene::Gameplay;
		case OPTION_INSTRUCTIONS:
			return game::Scene::Instructions;
		case OPTION_CREDITS:
			return game::Scene::Credits;
		case OPTION_EXIT:
			return game::Scene::Exit;
		default:
			return game::Scene::Menu;
		}
	}

	void draw()
	{
		ui::drawTextCentered("BREAKOUT", CENTER_X, 560.0, ui::TITLE_FONT_SIZE, colors::YELLOW);

		for (int i = 0; i < OPTION_COUNT; i++)
		{
			ui::drawButton(BUTTONS[i], i == selectedOption);
		}

		ui::drawMenuHint(60.0);
	}
}
