#include "entities/ball.h"

#include <cmath>

#include "sl.h"

#include "game/config.h"
#include "utils/color.h"

namespace ball
{
	namespace
	{
		const double RADIUS = 9.0;
		const double SPEED = 430.0;
		const int CIRCLE_VERTICES = 24;

		const double LAUNCH_DIRECTION_X = 0.5;
		const double LAUNCH_DIRECTION_Y = 0.866;
	}

	Ball create()
	{
		Ball ball;
		ball.x = 0.0;
		ball.y = 0.0;
		ball.radius = RADIUS;
		ball.speedX = 0.0;
		ball.speedY = 0.0;
		ball.isLaunched = false;

		return ball;
	}

	void placeOnPaddle(Ball& ball, const paddle::Paddle& paddle)
	{
		ball.x = paddle.x;
		ball.y = paddle.y + paddle.height / 2.0 + ball.radius + 1.0;
		ball.speedX = 0.0;
		ball.speedY = 0.0;
		ball.isLaunched = false;
	}

	void launch(Ball& ball)
	{
		ball.speedX = LAUNCH_DIRECTION_X * SPEED;
		ball.speedY = LAUNCH_DIRECTION_Y * SPEED;
		ball.isLaunched = true;
	}

	void update(Ball& ball, double deltaTime)
	{
		ball.x += ball.speedX * deltaTime;
		ball.y += ball.speedY * deltaTime;

		if (ball.x - ball.radius < 0.0)
		{
			ball.x = ball.radius;
			ball.speedX = std::fabs(ball.speedX);
		}

		if (ball.x + ball.radius > config::SCREEN_WIDTH)
		{
			ball.x = config::SCREEN_WIDTH - ball.radius;
			ball.speedX = -std::fabs(ball.speedX);
		}

		double topLimit = config::SCREEN_HEIGHT - config::HUD_HEIGHT;

		if (ball.y + ball.radius > topLimit)
		{
			ball.y = topLimit - ball.radius;
			ball.speedY = -std::fabs(ball.speedY);
		}
	}

	bool isOut(const Ball& ball)
	{
		return ball.y + ball.radius < 0.0;
	}

	void bounce(Ball& ball, double normalX, double normalY, double penetration)
	{
		ball.x += normalX * penetration;
		ball.y += normalY * penetration;

		if (std::fabs(normalX) > std::fabs(normalY))
		{
			if (ball.speedX * normalX < 0.0)
			{
				ball.speedX = -ball.speedX;
			}
		}
		else
		{
			if (ball.speedY * normalY < 0.0)
			{
				ball.speedY = -ball.speedY;
			}
		}
	}

	void draw(const Ball& ball)
	{
		colors::use(colors::WHITE);
		slCircleFill(ball.x, ball.y, ball.radius, CIRCLE_VERTICES);
	}
}
