#pragma once

#include "game/scene.h"

namespace gameplay
{
	void init();
	game::Scene update(double deltaTime);
	void draw();
}
