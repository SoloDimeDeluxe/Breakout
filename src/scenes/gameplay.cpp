#include "scenes/gameplay.h"

#include <string>

#include "sl.h"

#include "game/config.h"
#include "entities/paddle.h"
#include "entities/ball.h"
#include "entities/brick.h"
#include "utils/collisions.h"
#include "utils/input.h"
#include "utils/ui.h"

namespace gameplay
{
	namespace
	{
		enum class State
		{
			Playing,
			Paused,
			Won,
			Lost
		};

		const int START_LIVES = 3;
		const int POINTS_PER_BRICK = 10;

		const double CENTER_X = config::SCREEN_WIDTH / 2.0;
		const double CENTER_Y = config::SCREEN_HEIGHT / 2.0;

		const int PAUSE_OPTION_CONTINUE = 0;
		const int PAUSE_OPTION_MENU = 1;
		const int PAUSE_OPTION_COUNT = 2;
		const ui::Button PAUSE_BUTTONS[PAUSE_OPTION_COUNT] =
		{
			{ CENTER_X, CENTER_Y - 10.0, 260.0, 56.0, "Continuar" },
			{ CENTER_X, CENTER_Y - 90.0, 260.0, 56.0, "Menu" }
		};

		const int END_OPTION_RETRY = 0;
		const int END_OPTION_MENU = 1;
		const int END_OPTION_COUNT = 2;
		const ui::Button END_BUTTONS[END_OPTION_COUNT] =
		{
			{ CENTER_X, CENTER_Y - 10.0, 260.0, 56.0, "Jugar de nuevo" },
			{ CENTER_X, CENTER_Y - 90.0, 260.0, 56.0, "Menu" }
		};

		paddle::Paddle player;
		ball::Ball gameBall;
		brick::Brick bricks[brick::COUNT];

		State state = State::Playing;
		int lives = START_LIVES;
		int score = 0;
		int selectedOption = 0;

		void checkPaddleCollision()
		{
			if (gameBall.speedY >= 0.0)
			{
				return;
			}

			collisions::Result result = collisions::circleRect(
				gameBall.x, gameBall.y, gameBall.radius,
				player.x, player.y, player.width, player.height);

			if (result.hasCollided)
			{
				ball::bounce(gameBall, result.normalX, result.normalY, result.penetration);
			}
		}

		void checkBrickCollisions()
		{
			for (int i = 0; i < brick::COUNT; i++)
			{
				if (!bricks[i].isActive)
				{
					continue;
				}

				collisions::Result result = collisions::circleRect(
					gameBall.x, gameBall.y, gameBall.radius,
					bricks[i].x, bricks[i].y, bricks[i].width, bricks[i].height);

				if (result.hasCollided)
				{
					ball::bounce(gameBall, result.normalX, result.normalY, result.penetration);
					bricks[i].isActive = false;
					score += POINTS_PER_BRICK;
					return;
				}
			}
		}

		void updatePlaying(double deltaTime)
		{
			if (input::isKeyPressed('P') || input::isKeyPressed(SL_KEY_ESCAPE))
			{
				state = State::Paused;
				selectedOption = PAUSE_OPTION_CONTINUE;
				return;
			}

			paddle::update(player, deltaTime);

			if (!gameBall.isLaunched)
			{
				ball::placeOnPaddle(gameBall, player);

				if (input::isKeyPressed(' '))
				{
					ball::launch(gameBall);
				}
				return;
			}

			ball::update(gameBall, deltaTime);
			checkPaddleCollision();
			checkBrickCollisions();

			if (brick::countActive(bricks) == 0)
			{
				state = State::Won;
				selectedOption = END_OPTION_RETRY;
				return;
			}

			if (ball::isOut(gameBall))
			{
				lives--;

				if (lives <= 0)
				{
					state = State::Lost;
					selectedOption = END_OPTION_RETRY;
				}
				else
				{
					ball::placeOnPaddle(gameBall, player);
				}
			}
		}

		game::Scene updatePaused()
		{
			if (input::isKeyPressed('P') || input::isKeyPressed(SL_KEY_ESCAPE))
			{
				state = State::Playing;
				return game::Scene::Gameplay;
			}

			int chosen = ui::updateButtons(PAUSE_BUTTONS, PAUSE_OPTION_COUNT, selectedOption);

			if (chosen == PAUSE_OPTION_CONTINUE)
			{
				state = State::Playing;
			}
			else if (chosen == PAUSE_OPTION_MENU)
			{
				return game::Scene::Menu;
			}

			return game::Scene::Gameplay;
		}

		game::Scene updateEnd()
		{
			int chosen = ui::updateButtons(END_BUTTONS, END_OPTION_COUNT, selectedOption);

			if (chosen == END_OPTION_RETRY)
			{
				init();
			}
			else if (chosen == END_OPTION_MENU)
			{
				return game::Scene::Menu;
			}

			return game::Scene::Gameplay;
		}

		void drawHud()
		{
			double textY = config::SCREEN_HEIGHT - config::HUD_HEIGHT / 2.0 - 8.0;

			std::string livesText = "Vidas: " + std::to_string(lives);
			std::string scoreText = "Puntos: " + std::to_string(score);

			ui::drawText(livesText.c_str(), 20.0, textY, ui::TEXT_FONT_SIZE, colors::WHITE);
			ui::drawTextCentered("P / ESC: pausa", CENTER_X, textY, ui::TEXT_FONT_SIZE - 4.0, colors::GRAY);
			ui::drawTextRight(scoreText.c_str(), config::SCREEN_WIDTH - 20.0, textY, ui::TEXT_FONT_SIZE, colors::WHITE);

			colors::use(colors::GRAY);
			double lineY = config::SCREEN_HEIGHT - config::HUD_HEIGHT;
			slLine(0.0, lineY, config::SCREEN_WIDTH, lineY);
		}

		void drawPanel(const char* title, const Color& titleColor, const ui::Button buttons[], int count)
		{
			ui::drawOverlay();
			ui::drawTextCentered(title, CENTER_X, CENTER_Y + 90.0, ui::TITLE_FONT_SIZE - 8.0, titleColor);

			for (int i = 0; i < count; i++)
			{
				ui::drawButton(buttons[i], i == selectedOption);
			}
		}
	}

	void init()
	{
		player = paddle::create();
		gameBall = ball::create();
		ball::placeOnPaddle(gameBall, player);
		brick::initGrid(bricks);

		state = State::Playing;
		lives = START_LIVES;
		score = 0;
		selectedOption = 0;
	}

	game::Scene update(double deltaTime)
	{
		switch (state)
		{
		case State::Playing:
			updatePlaying(deltaTime);
			break;
		case State::Paused:
			return updatePaused();
		case State::Won:
		case State::Lost:
			return updateEnd();
		}

		return game::Scene::Gameplay;
	}

	void draw()
	{
		brick::drawAll(bricks);
		paddle::draw(player);
		ball::draw(gameBall);
		drawHud();

		if (state == State::Playing && !gameBall.isLaunched)
		{
			ui::drawTextCentered("ESPACIO para lanzar la pelota", CENTER_X, 150.0, ui::TEXT_FONT_SIZE, colors::WHITE);
			ui::drawTextCentered("Flechas / A D para mover", CENTER_X, 115.0, ui::TEXT_FONT_SIZE - 4.0, colors::GRAY);
		}

		switch (state)
		{
		case State::Paused:
			drawPanel("PAUSA", colors::WHITE, PAUSE_BUTTONS, PAUSE_OPTION_COUNT);
			break;
		case State::Won:
			drawPanel("GANASTE!", colors::YELLOW, END_BUTTONS, END_OPTION_COUNT);
			break;
		case State::Lost:
			drawPanel("PERDISTE", { 0.95, 0.3, 0.3, 1.0 }, END_BUTTONS, END_OPTION_COUNT);
			break;
		case State::Playing:
			break;
		}
	}
}
