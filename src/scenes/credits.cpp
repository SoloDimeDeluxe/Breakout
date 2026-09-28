#include "scenes/credits.h"

#include "sl.h"

#include "game/config.h"
#include "utils/input.h"
#include "utils/ui.h"

namespace credits
{
	namespace
	{
		const double CENTER_X = config::SCREEN_WIDTH / 2.0;

		const ui::Button BACK_BUTTON = { CENTER_X, 110.0, 260.0, 56.0, "Volver" };

		int selectedOption = 0;
	}

	void init()
	{
		selectedOption = 0;
	}

	game::Scene update()
	{
		if (input::isKeyPressed(SL_KEY_ESCAPE))
		{
			return game::Scene::Menu;
		}

		if (ui::updateButtons(&BACK_BUTTON, 1, selectedOption) == 0)
		{
			return game::Scene::Menu;
		}

		return game::Scene::Credits;
	}

	void draw()
	{
		ui::drawTextCentered("CREDITOS", CENTER_X, 620.0, ui::TITLE_FONT_SIZE - 16.0, colors::YELLOW);

		ui::drawTextCentered("Programacion y diseno", CENTER_X, 520.0, ui::TEXT_FONT_SIZE, colors::GRAY);
		ui::drawTextCentered("Valentin Reyes", CENTER_X, 480.0, ui::BUTTON_FONT_SIZE, colors::WHITE);

		ui::drawTextCentered("Libreria grafica", CENTER_X, 400.0, ui::TEXT_FONT_SIZE, colors::GRAY);
		ui::drawTextCentered("SIGIL - Geoff Nagy", CENTER_X, 360.0, ui::BUTTON_FONT_SIZE, colors::WHITE);

		ui::drawTextCentered("Fuente", CENTER_X, 280.0, ui::TEXT_FONT_SIZE, colors::GRAY);
		ui::drawTextCentered("DejaVu Sans", CENTER_X, 240.0, ui::BUTTON_FONT_SIZE, colors::WHITE);

		ui::drawButton(BACK_BUTTON, true);

		ui::drawTextCentered("ESC o ENTER para volver al menu", CENTER_X, 50.0, ui::TEXT_FONT_SIZE - 4.0, colors::GRAY);
	}
}
