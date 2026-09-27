#pragma once

#include "utils/color.h"

namespace brick
{
	const int ROWS = 6;
	const int COLUMNS = 10;
	const int COUNT = ROWS * COLUMNS;

	struct Brick
	{
		double x;
		double y;
		double width;
		double height;
		Color color;
		bool isActive;
	};

	void initGrid(Brick bricks[]);

	int countActive(const Brick bricks[]);

	void drawAll(const Brick bricks[]);
}
