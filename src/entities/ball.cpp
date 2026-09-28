#include "entities/ball.h"

#include <cmath>

#include "sl.h"

#include "game/config.h"
#include "utils/color.h"
#include "utils/textures.h"

namespace ball
{
	namespace
	{
		const double RADIUS = 9.0;
		const double SPEED = 430.0;

		const double LAUNCH_DIRECTION_X = 0.5;
		const double LAUNCH_DIRECTION_Y = 0.866;

		const double PI = 3.14159265358979;
		const double MAX_BOUNCE_ANGLE = 60.0 * PI / 180.0;
	}

	Ball create()
	{
		Ball ball;
		ball.x = 0.0;
		ball.y = 0.0;
		ball.radius = RADIUS;
		ball.speedX = 0.0;
		ball.speedY = 0.0;
		ball.speedScale = 1.0;
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
		ball.speedX = LAUNCH_DIRECTION_X * SPEED * ball.speedScale;
		ball.speedY = LAUNCH_DIRECTION_Y * SPEED * ball.speedScale;
		ball.isLaunched = true;
	}

	void setSpeedScale(Ball& ball, double scale)
	{
		double currentSpeed = std::sqrt(ball.speedX * ball.speedX + ball.speedY * ball.speedY);

		if (currentSpeed > 0.0)
		{
			double newSpeed = SPEED * scale;
			ball.speedX = ball.speedX / currentSpeed * newSpeed;
			ball.speedY = ball.speedY / currentSpeed * newSpeed;
		}

		ball.speedScale = scale;
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

	void bounceOffPaddle(Ball& ball, const paddle::Paddle& paddle)
	{
		double halfWidth = paddle.width / 2.0;
		double hitOffset = (ball.x - paddle.x) / halfWidth;

		if (hitOffset < -1.0)
		{
			hitOffset = -1.0;
		}
		if (hitOffset > 1.0)
		{
			hitOffset = 1.0;
		}

		double angle = hitOffset * MAX_BOUNCE_ANGLE;

		ball.speedX = SPEED * ball.speedScale * std::sin(angle);
		ball.speedY = SPEED * ball.speedScale * std::cos(angle);

		ball.y = paddle.y + paddle.height / 2.0 + ball.radius;
	}

	void draw(const Ball& ball)
	{
		colors::use(colors::WHITE);
		slSprite(textures::getBall(), ball.x, ball.y, ball.radius * 2.0, ball.radius * 2.0);
	}
}
