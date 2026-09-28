#include "utils/collisions.h"

#include <cmath>

namespace collisions
{
	namespace
	{
		double clamp(double value, double min, double max)
		{
			if (value < min)
			{
				return min;
			}
			if (value > max)
			{
				return max;
			}
			return value;
		}
	}

	bool rectRect(double x1, double y1, double width1, double height1,
		double x2, double y2, double width2, double height2)
	{
		return std::fabs(x1 - x2) < (width1 + width2) / 2.0 &&
			std::fabs(y1 - y2) < (height1 + height2) / 2.0;
	}

	Result circleRect(double circleX, double circleY, double radius,
		double rectX, double rectY, double rectWidth, double rectHeight)
	{
		Result result = { false, 0.0, 0.0, 0.0 };

		double halfWidth = rectWidth / 2.0;
		double halfHeight = rectHeight / 2.0;

		double closestX = clamp(circleX, rectX - halfWidth, rectX + halfWidth);
		double closestY = clamp(circleY, rectY - halfHeight, rectY + halfHeight);

		double distanceX = circleX - closestX;
		double distanceY = circleY - closestY;
		double distanceSquared = distanceX * distanceX + distanceY * distanceY;

		if (distanceSquared > radius * radius)
		{
			return result;
		}

		result.hasCollided = true;

		if (distanceSquared > 0.0)
		{
			double distance = std::sqrt(distanceSquared);
			result.normalX = distanceX / distance;
			result.normalY = distanceY / distance;
			result.penetration = radius - distance;
		}
		else
		{
			double overlapX = halfWidth - std::fabs(circleX - rectX);
			double overlapY = halfHeight - std::fabs(circleY - rectY);

			if (overlapX < overlapY)
			{
				result.normalX = (circleX < rectX) ? -1.0 : 1.0;
				result.penetration = overlapX + radius;
			}
			else
			{
				result.normalY = (circleY < rectY) ? -1.0 : 1.0;
				result.penetration = overlapY + radius;
			}
		}

		return result;
	}
}
