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
