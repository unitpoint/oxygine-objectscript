#include "oxygine-framework.h"
#include "res.h"
#include "Game.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "GameMenu.h"
#include <ox-binder.h>

using namespace oxygine;

void example_preinit()
{
	ObjectScript::Oxygine::init();
}

void example_init()
{
	if(0){
		//load resources
		res::load();

		//create all scenes
		GameMenu::instance = new GameMenu;
		GameScene::instance = new GameScene;
		MainMenuScene::instance = new MainMenuScene;

		//show main menu
		MainMenuScene::instance->show();
	}else{
		ObjectScript::Oxygine::run();
	}
}

void example_update()
{
	sleep(10);
}

void example_destroy()
{
	MainMenuScene::instance = 0;
	GameScene::instance = 0;
	GameMenu::instance = 0;

	ObjectScript::Oxygine::shutdown();

	res::free();
}