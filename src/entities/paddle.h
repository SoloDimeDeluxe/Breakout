#pragma once

namespace paddle
{
	struct Paddle
	{
		double x;
		double y;
		double width;
		double height;
		double speed;
	};

	Paddle create();

	void update(Paddle& paddle, double deltaTime);

	void setWidthScale(Paddle& paddle, double scale);

	void draw(const Paddle& paddle);
}
