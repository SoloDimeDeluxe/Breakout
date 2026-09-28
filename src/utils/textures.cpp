#include "utils/textures.h"

#include "sl.h"

#include "game/config.h"
#include "utils/color.h"

namespace textures
{
	namespace
	{
		const char* const BRICK_PATH = "res/textures/brick.png";
		const char* const PADDLE_PATH = "res/textures/paddle.png";
		const char* const BALL_PATH = "res/textures/ball.png";
		const char* const POWER_UP_PATH = "res/textures/powerup.png";
		const char* const BACKGROUND_PATH = "res/textures/background.png";

		int brick = -1;
		int paddle = -1;
		int ball = -1;
		int powerUp = -1;
		int background = -1;
	}

	void load()
	{
		brick = slLoadTexture(BRICK_PATH);
		paddle = slLoadTexture(PADDLE_PATH);
		ball = slLoadTexture(BALL_PATH);
		powerUp = slLoadTexture(POWER_UP_PATH);
		background = slLoadTexture(BACKGROUND_PATH);
	}

	int getBrick()
	{
		return brick;
	}

	int getPaddle()
	{
		return paddle;
	}

	int getBall()
	{
		return ball;
	}

	int getPowerUp()
	{
		return powerUp;
	}

	void drawBackground()
	{
		colors::use(colors::WHITE);
		slSprite(background, config::SCREEN_WIDTH / 2.0, config::SCREEN_HEIGHT / 2.0,
			config::SCREEN_WIDTH, config::SCREEN_HEIGHT);
	}
}
