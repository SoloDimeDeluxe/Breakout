#pragma once

#include "entities/paddle.h"

namespace ball
{
	struct Ball
	{
		double x;
		double y;
		double radius;
		double speedX;
		double speedY;
		double speedScale;
		bool isLaunched;
	};

	Ball create();

	void placeOnPaddle(Ball& ball, const paddle::Paddle& paddle);

	void launch(Ball& ball);

	void setSpeedScale(Ball& ball, double scale);

	void update(Ball& ball, double deltaTime);

	bool isOut(const Ball& ball);

	void bounce(Ball& ball, double normalX, double normalY, double penetration);

	void bounceOffPaddle(Ball& ball, const paddle::Paddle& paddle);

	void draw(const Ball& ball);
}
