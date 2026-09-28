#include "scenes/gameplay.h"

#include <string>

#include "sl.h"

#include "game/config.h"
#include "entities/paddle.h"
#include "entities/ball.h"
#include "entities/brick.h"
#include "entities/powerup.h"
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
		const double END_DELAY = 1.5;
		const int MAX_LIVES = 5;
		const double EFFECT_DURATION = 10.0;
		const double WIDE_PADDLE_SCALE = 1.5;
		const double SLOW_BALL_SCALE = 0.65;
		const Color LOST_COLOR = { 0.95, 0.3, 0.3, 1.0 };

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
		powerup::PowerUp powerUps[powerup::MAX_COUNT];

		State state = State::Playing;
		int lives = START_LIVES;
		int score = 0;
		int selectedOption = 0;
		double endTimer = 0.0;
		double widePaddleTimer = 0.0;
		double slowBallTimer = 0.0;

		void resetEffects()
		{
			widePaddleTimer = 0.0;
			slowBallTimer = 0.0;
			paddle::setWidthScale(player, 1.0);
			ball::setSpeedScale(gameBall, 1.0);
		}

		void applyPowerUp(powerup::Type type)
		{
			switch (type)
			{
			case powerup::Type::WidePaddle:
				widePaddleTimer = EFFECT_DURATION;
				paddle::setWidthScale(player, WIDE_PADDLE_SCALE);
				break;
			case powerup::Type::SlowBall:
				slowBallTimer = EFFECT_DURATION;
				ball::setSpeedScale(gameBall, SLOW_BALL_SCALE);
				break;
			case powerup::Type::ExtraLife:
				if (lives < MAX_LIVES)
				{
					lives++;
				}
				break;
			case powerup::Type::Count:
				break;
			}
		}

		void updateEffects(double deltaTime)
		{
			if (widePaddleTimer > 0.0)
			{
				widePaddleTimer -= deltaTime;

				if (widePaddleTimer <= 0.0)
				{
					widePaddleTimer = 0.0;
					paddle::setWidthScale(player, 1.0);
				}
			}

			if (slowBallTimer > 0.0)
			{
				slowBallTimer -= deltaTime;

				if (slowBallTimer <= 0.0)
				{
					slowBallTimer = 0.0;
					ball::setSpeedScale(gameBall, 1.0);
				}
			}
		}

		void checkPowerUpCatch()
		{
			for (int i = 0; i < powerup::MAX_COUNT; i++)
			{
				powerup::PowerUp& current = powerUps[i];

				if (current.isActive &&
					collisions::rectRect(current.x, current.y, current.width, current.height,
						player.x, player.y, player.width, player.height))
				{
					applyPowerUp(current.type);
					current.isActive = false;
				}
			}
		}

		void finishMatch(State result)
		{
			state = result;
			selectedOption = END_OPTION_RETRY;
			endTimer = END_DELAY;
		}

		void checkPaddleCollision()
		{
			if (gameBall.speedY >= 0.0)
			{
				return;
			}

			collisions::Result result = collisions::circleRect(
				gameBall.x, gameBall.y, gameBall.radius,
				player.x, player.y, player.width, player.height);

			if (!result.hasCollided)
			{
				return;
			}

			if (gameBall.y >= player.y)
			{
				ball::bounceOffPaddle(gameBall, player);
			}
			else
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
					powerup::trySpawn(powerUps, bricks[i].x, bricks[i].y);
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
			powerup::updateAll(powerUps, deltaTime);
			checkPowerUpCatch();
			updateEffects(deltaTime);

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
				finishMatch(State::Won);
				return;
			}

			if (ball::isOut(gameBall))
			{
				lives--;

				if (lives <= 0)
				{
					finishMatch(State::Lost);
				}
				else
				{
					resetEffects();
					powerup::clearAll(powerUps);
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

		game::Scene updateEnd(double deltaTime)
		{
			if (endTimer > 0.0)
			{
				endTimer -= deltaTime;
				return game::Scene::Gameplay;
			}

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

			colors::use(colors::SHADOW);
			slRectangleFill(CENTER_X, config::SCREEN_HEIGHT - config::HUD_HEIGHT / 2.0,
				config::SCREEN_WIDTH, config::HUD_HEIGHT);

			std::string livesText = "Vidas: " + std::to_string(lives);
			std::string scoreText = "Puntos: " + std::to_string(score);

			ui::drawText(livesText.c_str(), 20.0, textY, ui::TEXT_FONT_SIZE, colors::WHITE);
			ui::drawTextCentered("P / ESC: pausa", CENTER_X, textY, ui::TEXT_FONT_SIZE - 4.0, colors::GRAY);
			ui::drawTextRight(scoreText.c_str(), config::SCREEN_WIDTH - 20.0, textY, ui::TEXT_FONT_SIZE, colors::WHITE);

			colors::use(colors::GRAY);
			double lineY = config::SCREEN_HEIGHT - config::HUD_HEIGHT;
			slLine(0.0, lineY, config::SCREEN_WIDTH, lineY);

			double effectY = lineY - 25.0;

			if (widePaddleTimer > 0.0)
			{
				std::string wideText = "Paleta ancha: " + std::to_string(static_cast<int>(widePaddleTimer) + 1) + "s";
				ui::drawText(wideText.c_str(), 20.0, effectY, ui::TEXT_FONT_SIZE - 6.0, colors::WHITE);
			}

			if (slowBallTimer > 0.0)
			{
				std::string slowText = "Pelota lenta: " + std::to_string(static_cast<int>(slowBallTimer) + 1) + "s";
				ui::drawTextRight(slowText.c_str(), config::SCREEN_WIDTH - 20.0, effectY, ui::TEXT_FONT_SIZE - 6.0, colors::WHITE);
			}
		}

		void drawPanel(const char* title, const Color& titleColor, const ui::Button buttons[], int count)
		{
			ui::drawOverlay();
			ui::drawTextCentered(title, CENTER_X, CENTER_Y + 130.0, ui::TITLE_FONT_SIZE - 8.0, titleColor);

			for (int i = 0; i < count; i++)
			{
				ui::drawButton(buttons[i], i == selectedOption);
			}

			ui::drawMenuHint(CENTER_Y - 170.0);
		}

		void drawEndPanel(const char* title, const Color& titleColor, const char* reason)
		{
			drawPanel(title, titleColor, END_BUTTONS, END_OPTION_COUNT);

			std::string scoreText = "Puntaje final: " + std::to_string(score);

			ui::drawTextCentered(reason, CENTER_X, CENTER_Y + 80.0, ui::TEXT_FONT_SIZE, colors::WHITE);
			ui::drawTextCentered(scoreText.c_str(), CENTER_X, CENTER_Y + 45.0, ui::TEXT_FONT_SIZE, colors::YELLOW);
		}
	}

	void init()
	{
		player = paddle::create();
		gameBall = ball::create();
		ball::placeOnPaddle(gameBall, player);
		brick::initGrid(bricks);
		powerup::clearAll(powerUps);

		state = State::Playing;
		lives = START_LIVES;
		score = 0;
		selectedOption = 0;
		endTimer = 0.0;
		widePaddleTimer = 0.0;
		slowBallTimer = 0.0;
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
			return updateEnd(deltaTime);
		}

		return game::Scene::Gameplay;
	}

	void draw()
	{
		brick::drawAll(bricks);
		powerup::drawAll(powerUps);
		paddle::draw(player);
		ball::draw(gameBall);
		drawHud();

		if (state == State::Playing && !gameBall.isLaunched)
		{
			ui::drawTextCentered("Rompe todos los ladrillos para ganar", CENTER_X, 190.0, ui::TEXT_FONT_SIZE, colors::YELLOW);
			ui::drawTextCentered("ESPACIO para lanzar la pelota", CENTER_X, 150.0, ui::TEXT_FONT_SIZE, colors::WHITE);
			ui::drawTextCentered("Flechas / A D para mover", CENTER_X, 115.0, ui::TEXT_FONT_SIZE - 4.0, colors::GRAY);
		}

		bool isEndPanelVisible = endTimer <= 0.0;

		switch (state)
		{
		case State::Paused:
			drawPanel("PAUSA", colors::WHITE, PAUSE_BUTTONS, PAUSE_OPTION_COUNT);
			ui::drawTextCentered("P / ESC para continuar", CENTER_X, CENTER_Y + 70.0, ui::TEXT_FONT_SIZE, colors::GRAY);
			break;
		case State::Won:
			if (isEndPanelVisible)
			{
				drawEndPanel("GANASTE!", colors::YELLOW, "Rompiste todos los ladrillos");
			}
			break;
		case State::Lost:
			if (isEndPanelVisible)
			{
				drawEndPanel("PERDISTE", LOST_COLOR, "Te quedaste sin vidas");
			}
			break;
		case State::Playing:
			break;
		}
	}
}
