#include "scenes/instructions.h"

#include "sl.h"

#include "game/config.h"
#include "entities/powerup.h"
#include "utils/input.h"
#include "utils/ui.h"

namespace instructions
{
	namespace
	{
		const double CENTER_X = config::SCREEN_WIDTH / 2.0;
		const double LEFT_X = 150.0;

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

		return game::Scene::Instructions;
	}

	void draw()
	{
		ui::drawTextCentered("COMO JUGAR", CENTER_X, 700.0, ui::TITLE_FONT_SIZE - 16.0, colors::YELLOW);

		ui::drawText("Objetivo", LEFT_X, 640.0, ui::BUTTON_FONT_SIZE, colors::YELLOW);
		ui::drawText("Rompe todos los ladrillos con la pelota para ganar.", LEFT_X, 608.0, ui::TEXT_FONT_SIZE, colors::WHITE);
		ui::drawText("Si la pelota cae debajo de la paleta, perdes una vida.", LEFT_X, 580.0, ui::TEXT_FONT_SIZE, colors::WHITE);
		ui::drawText("Tenes 3 vidas: si las perdes todas, perdes la partida.", LEFT_X, 552.0, ui::TEXT_FONT_SIZE, colors::WHITE);

		ui::drawText("Controles", LEFT_X, 505.0, ui::BUTTON_FONT_SIZE, colors::YELLOW);
		ui::drawText("Flechas izquierda / derecha o A / D: mover la paleta", LEFT_X, 473.0, ui::TEXT_FONT_SIZE, colors::WHITE);
		ui::drawText("ESPACIO: lanzar la pelota", LEFT_X, 445.0, ui::TEXT_FONT_SIZE, colors::WHITE);
		ui::drawText("P o ESC: pausa", LEFT_X, 417.0, ui::TEXT_FONT_SIZE, colors::WHITE);

		ui::drawText("Power-ups", LEFT_X, 370.0, ui::BUTTON_FONT_SIZE, colors::YELLOW);
		ui::drawText("Al romper un ladrillo puede caer uno: agarralo con la paleta.", LEFT_X, 338.0, ui::TEXT_FONT_SIZE, colors::WHITE);
		ui::drawText("A (verde): paleta mas ancha por 10 segundos", LEFT_X, 310.0, ui::TEXT_FONT_SIZE, powerup::getColor(powerup::Type::WidePaddle));
		ui::drawText("L (celeste): pelota mas lenta por 10 segundos", LEFT_X, 282.0, ui::TEXT_FONT_SIZE, powerup::getColor(powerup::Type::SlowBall));
		ui::drawText("+1 (rosa): una vida extra", LEFT_X, 254.0, ui::TEXT_FONT_SIZE, powerup::getColor(powerup::Type::ExtraLife));

		ui::drawText("Consejo: el angulo de la pelota depende de donde pega en la paleta.", LEFT_X, 200.0, ui::TEXT_FONT_SIZE - 2.0, colors::GRAY);

		ui::drawButton(BACK_BUTTON, true);

		ui::drawTextCentered("ESC o ENTER para volver al menu", CENTER_X, 50.0, ui::TEXT_FONT_SIZE - 4.0, colors::GRAY);
	}
}
