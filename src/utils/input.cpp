#include "utils/input.h"

#include "sl.h"

namespace input
{
	namespace
	{
		const int KEY_COUNT = 512;

		bool currentKeys[KEY_COUNT] = {};
		bool previousKeys[KEY_COUNT] = {};

		bool currentMouseLeft = false;
		bool previousMouseLeft = false;

		bool isValidKey(int key)
		{
			return key >= 0 && key < KEY_COUNT;
		}
	}

	void update()
	{
		for (int key = 0; key < KEY_COUNT; key++)
		{
			previousKeys[key] = currentKeys[key];
			currentKeys[key] = slGetKey(key) != 0;
		}

		previousMouseLeft = currentMouseLeft;
		currentMouseLeft = slGetMouseButton(SL_MOUSE_BUTTON_LEFT) != 0;
	}

	bool isKeyDown(int key)
	{
		return isValidKey(key) && currentKeys[key];
	}

	bool isKeyPressed(int key)
	{
		return isValidKey(key) && currentKeys[key] && !previousKeys[key];
	}

	bool isMouseLeftPressed()
	{
		return currentMouseLeft && !previousMouseLeft;
	}

	double getMouseX()
	{
		return slGetMouseX();
	}

	double getMouseY()
	{
		return slGetMouseY();
	}
}
