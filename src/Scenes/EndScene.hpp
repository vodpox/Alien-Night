#ifndef ENDSCENE
#define ENDSCENE


#include "Game.h"
#include "Scene.h"
#include "Camera.h"


class EndScene : public tplay::Scene {
	public:
		tplay::Game *game;
		tplay::Camera camera;
		
		tplay::Scene *menuScene;
		
		int selectedOption = 0;
		int optionCount = 3;
		
		int highscore = 0;
		bool newHighscore = false;
		
		bool hasWon = false;
		int score = 0;
		
		EndScene(tplay::Game *game);
		void setScenes(tplay::Scene *menuScene);
		void setData(bool hasWon, int score);
		
		int getHisghscore();
		void checkHisghscore();
		
		void update();
		void draw();
		
		
};

#endif
