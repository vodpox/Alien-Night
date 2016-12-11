#ifndef MENUSCENE
#define MENUSCENE


#include "Game.h"
#include "Scene.h"
#include "Camera.h"
#include "PlayScene.hpp"


class MenuScene : public tplay::Scene {
	public:
		tplay::Game *game;
		tplay::Camera camera;
		
		PlayScene *playScene;
		tplay::Scene *tutorialScene;
		EndScene *endScene;
		
		bool rip = false;
		
		int selectedOption = 0;
		int optionCount = 3;
		
		MenuScene(tplay::Game *game);
		void setScenes(tplay::Scene *tutorialScene, EndScene *endScene);
		
		void update();
		void draw();
		
		
};

#endif
