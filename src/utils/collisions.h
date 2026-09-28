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

	bool rectRect(double x1, double y1, double width1, double height1,
		double x2, double y2, double width2, double height2);

	Result circleRect(double circleX, double circleY, double radius,
		double rectX, double rectY, double rectWidth, double rectHeight);
}
