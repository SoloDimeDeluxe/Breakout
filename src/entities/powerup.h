#pragma once

#include "utils/color.h"

namespace powerup
{
	enum class Type
	{
		WidePaddle,
		SlowBall,
		ExtraLife,
		Count
	};

	struct PowerUp
	{
		double x;
		double y;
		double width;
		double height;
		double speed;
		Type type;
		bool isActive;
	};

	const int MAX_COUNT = 8;

	void clearAll(PowerUp powerUps[]);

	void trySpawn(PowerUp powerUps[], double x, double y);

	void updateAll(PowerUp powerUps[], double deltaTime);

	void drawAll(const PowerUp powerUps[]);

	const Color& getColor(Type type);
}
