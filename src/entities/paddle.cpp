#include "entities/paddle.h"

#include "sl.h"

#include "game/config.h"
#include "utils/color.h"
#include "utils/input.h"

namespace paddle
{
	namespace
	{
		const double WIDTH = 130.0;
		const double HEIGHT = 18.0;
		const double POSITION_Y = 50.0;
		const double SPEED = 650.0;

		const Color COLOR = { 0.3, 0.8, 1.0, 1.0 };
	}

	Paddle create()
	{
		Paddle paddle;
		paddle.x = config::SCREEN_WIDTH / 2.0;
		paddle.y = POSITION_Y;
		paddle.width = WIDTH;
		paddle.height = HEIGHT;
		paddle.speed = SPEED;

		return paddle;
	}

	void update(Paddle& paddle, double deltaTime)
	{
		if (input::isKeyDown(SL_KEY_LEFT) || input::isKeyDown('A'))
		{
			paddle.x -= paddle.speed * deltaTime;
		}
		if (input::isKeyDown(SL_KEY_RIGHT) || input::isKeyDown('D'))
		{
			paddle.x += paddle.speed * deltaTime;
		}

		double halfWidth = paddle.width / 2.0;

		if (paddle.x - halfWidth < 0.0)
		{
			paddle.x = halfWidth;
		}
		if (paddle.x + halfWidth > config::SCREEN_WIDTH)
		{
			paddle.x = config::SCREEN_WIDTH - halfWidth;
		}
	}

	void draw(const Paddle& paddle)
	{
		colors::use(COLOR);
		slRectangleFill(paddle.x, paddle.y, paddle.width, paddle.height);
	}
}
