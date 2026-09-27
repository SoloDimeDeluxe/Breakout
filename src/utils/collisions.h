#pragma once

namespace collisions
{
	struct Result
	{
		bool hasCollided;
		double normalX;
		double normalY;
		double penetration;
	};

	Result circleRect(double circleX, double circleY, double radius,
		double rectX, double rectY, double rectWidth, double rectHeight);
}
