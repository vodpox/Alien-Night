#ifndef TUTORIALSCENE
#define TUTORIALSCENE


#include "Game.h"
#include "Scene.h"
#include "Camera.h"


class TutorialScene : public tplay::Scene {
	public:
		tplay::Game *game;
		tplay::Camera camera;
		
		tplay::Scene *menuScene;
		
		int selectedOption = 0;
		int optionCount = 3;
		
		TutorialScene(tplay::Game *game);
		void setScenes(tplay::Scene *menuScene);
		
		void update();
		void draw();
		
		
};

#endif
