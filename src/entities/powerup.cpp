#include "entities/powerup.h"

#include <cstdlib>

#include "sl.h"

#include "utils/color.h"
#include "utils/textures.h"
#include "utils/ui.h"

namespace powerup
{
	namespace
	{
		const double WIDTH = 44.0;
		const double HEIGHT = 22.0;
		const double FALL_SPEED = 160.0;
		const int SPAWN_CHANCE_PERCENT = 20;
		const double LABEL_FONT_SIZE = 16.0;

		const Color WIDE_PADDLE_COLOR = { 0.35, 0.85, 0.45, 1.0 };
		const Color SLOW_BALL_COLOR = { 0.35, 0.8, 0.95, 1.0 };
		const Color EXTRA_LIFE_COLOR = { 0.95, 0.45, 0.75, 1.0 };

		const char* getLabel(Type type)
		{
			switch (type)
			{
			case Type::WidePaddle:
				return "A";
			case Type::SlowBall:
				return "L";
			default:
				return "+1";
			}
		}
	}

	const Color& getColor(Type type)
	{
		switch (type)
		{
		case Type::WidePaddle:
			return WIDE_PADDLE_COLOR;
		case Type::SlowBall:
			return SLOW_BALL_COLOR;
		default:
			return EXTRA_LIFE_COLOR;
		}
	}

	void clearAll(PowerUp powerUps[])
	{
		for (int i = 0; i < MAX_COUNT; i++)
		{
			powerUps[i].isActive = false;
		}
	}

	void trySpawn(PowerUp powerUps[], double x, double y)
	{
		if (std::rand() % 100 >= SPAWN_CHANCE_PERCENT)
		{
			return;
		}

		for (int i = 0; i < MAX_COUNT; i++)
		{
			if (!powerUps[i].isActive)
			{
				powerUps[i].x = x;
				powerUps[i].y = y;
				powerUps[i].width = WIDTH;
				powerUps[i].height = HEIGHT;
				powerUps[i].speed = FALL_SPEED;
				powerUps[i].type = static_cast<Type>(std::rand() % static_cast<int>(Type::Count));
				powerUps[i].isActive = true;
				return;
			}
		}
	}

	void updateAll(PowerUp powerUps[], double deltaTime)
	{
		for (int i = 0; i < MAX_COUNT; i++)
		{
			if (!powerUps[i].isActive)
			{
				continue;
			}

			powerUps[i].y -= powerUps[i].speed * deltaTime;

			if (powerUps[i].y + powerUps[i].height / 2.0 < 0.0)
			{
				powerUps[i].isActive = false;
			}
		}
	}

	void drawAll(const PowerUp powerUps[])
	{
		for (int i = 0; i < MAX_COUNT; i++)
		{
			const PowerUp& current = powerUps[i];

			if (!current.isActive)
			{
				continue;
			}

			colors::use(getColor(current.type));
			slSprite(textures::getPowerUp(), current.x, current.y, current.width, current.height);

			ui::drawTextCentered(getLabel(current.type), current.x, current.y - LABEL_FONT_SIZE / 3.0,
				LABEL_FONT_SIZE, colors::DARK_GRAY);
		}
	}
}
