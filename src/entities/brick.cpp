#include "entities/brick.h"

#include "sl.h"

#include "game/config.h"

namespace brick
{
	namespace
	{
		const double WIDTH = 88.0;
		const double HEIGHT = 26.0;
		const double GAP = 8.0;

		const double TOP_MARGIN = 50.0;

		const Color ROW_COLORS[ROWS] =
		{
			{ 0.95, 0.30, 0.30, 1.0 },
			{ 0.95, 0.55, 0.25, 1.0 },
			{ 0.95, 0.85, 0.30, 1.0 },
			{ 0.40, 0.85, 0.40, 1.0 },
			{ 0.30, 0.65, 0.95, 1.0 },
			{ 0.65, 0.45, 0.95, 1.0 }
		};
	}

	void initGrid(Brick bricks[])
	{
		double gridWidth = COLUMNS * WIDTH + (COLUMNS - 1) * GAP;
		double startX = (config::SCREEN_WIDTH - gridWidth) / 2.0 + WIDTH / 2.0;
		double startY = config::SCREEN_HEIGHT - config::HUD_HEIGHT - TOP_MARGIN - HEIGHT / 2.0;

		for (int row = 0; row < ROWS; row++)
		{
			for (int column = 0; column < COLUMNS; column++)
			{
				Brick& current = bricks[row * COLUMNS + column];

				current.x = startX + column * (WIDTH + GAP);
				current.y = startY - row * (HEIGHT + GAP);
				current.width = WIDTH;
				current.height = HEIGHT;
				current.color = ROW_COLORS[row];
				current.isActive = true;
			}
		}
	}

	int countActive(const Brick bricks[])
	{
		int count = 0;

		for (int i = 0; i < COUNT; i++)
		{
			if (bricks[i].isActive)
			{
				count++;
			}
		}

		return count;
	}

	void drawAll(const Brick bricks[])
	{
		for (int i = 0; i < COUNT; i++)
		{
			if (bricks[i].isActive)
			{
				colors::use(bricks[i].color);
				slRectangleFill(bricks[i].x, bricks[i].y, bricks[i].width, bricks[i].height);
			}
		}
	}
}
