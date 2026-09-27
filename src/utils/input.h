#pragma once

namespace input
{
	void update();

	bool isKeyDown(int key);
	bool isKeyPressed(int key);

	bool isMouseLeftPressed();
	double getMouseX();
	double getMouseY();
}
