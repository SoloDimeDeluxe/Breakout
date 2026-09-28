#include "game/game.h"

#include <cstdlib>
#include <ctime>

#include "sl.h"

#include "game/config.h"
#include "game/scene.h"
#include "scenes/menu.h"
#include "scenes/gameplay.h"
#include "scenes/instructions.h"
#include "scenes/credits.h"
#include "utils/input.h"
#include "utils/textures.h"
#include "utils/ui.h"

namespace game
{
	namespace
	{
		void enterScene(Scene scene)
		{
			switch (scene)
			{
			case Scene::Menu:
				menu::init();
				break;
			case Scene::Gameplay:
				gameplay::init();
				break;
			case Scene::Instructions:
				instructions::init();
				break;
			case Scene::Credits:
				credits::init();
				break;
			case Scene::Exit:
				break;
			}
		}

		Scene updateScene(Scene scene, double deltaTime)
		{
			switch (scene)
			{
			case Scene::Menu:
				return menu::update();
			case Scene::Gameplay:
				return gameplay::update(deltaTime);
			case Scene::Instructions:
				return instructions::update();
			case Scene::Credits:
				return credits::update();
			case Scene::Exit:
				break;
			}

			return scene;
		}

		void drawScene(Scene scene)
		{
			switch (scene)
			{
			case Scene::Menu:
				menu::draw();
				break;
			case Scene::Gameplay:
				gameplay::draw();
				break;
			case Scene::Instructions:
				instructions::draw();
				break;
			case Scene::Credits:
				credits::draw();
				break;
			case Scene::Exit:
				break;
			}
		}
	}

	void run()
	{
		slWindow(config::SCREEN_WIDTH, config::SCREEN_HEIGHT, config::WINDOW_TITLE, false);
		slSetBackColor(0.06, 0.06, 0.1);

		ui::init();
		textures::load();

		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		Scene currentScene = Scene::Menu;
		enterScene(currentScene);

		while (!slShouldClose() && currentScene != Scene::Exit)
		{
			double deltaTime = slGetDeltaTime();
			if (deltaTime > config::MAX_DELTA_TIME)
			{
				deltaTime = config::MAX_DELTA_TIME;
			}

			input::update();

			Scene nextScene = updateScene(currentScene, deltaTime);
			textures::drawBackground();
			drawScene(currentScene);

			slRender();

			if (nextScene != currentScene)
			{
				currentScene = nextScene;
				enterScene(currentScene);
			}
		}

		slClose();
	}
}
