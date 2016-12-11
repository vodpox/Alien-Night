#ifndef PLAYSCNE
#define PLAYSCNE


#include "Game.h"
#include "Scene.h"
#include "Camera.h"
#include "../Player.hpp"
#include "../Portal.hpp"
#include "../Enemy.hpp"
#include "EndScene.hpp"
#include <vector>


class PlayScene : public tplay::Scene {
	private:
		
		// map things
		int mapX, mapY;
		void loadMap(int level);
		char Map[1000][1000]; // TODO do map with malloc and all that
		//char *mapPtr;
		bool collides(int x, int y, bool notOnlyWalls);
		
		// ayyliens
		std::vector<Enemy*> Enemies;
		std::vector<Portal*> Portals;
		
		// shooting things
		bool aiming = false;
		int aimX, aimY;
		int currentWeapon = 0;
		int weaponCount = 3;
		int WeaponRange[3] = {50, 25, 10};
		int grenadeRange = 5;
		
		bool endTurn = false;
		bool shootingAnimation = false;
		int animationProgress = 0;
		int animationStage = 0;
		
		bool inRange(int x1, int y1, int x2, int y2, int range);
		void resetAim();
		
		int score = 0;
	
	public:
		tplay::Game *game;
		tplay::Camera camera;
		
		tplay::Scene *menuScene;
		EndScene *endScene;
		
		int level;
		
		int selectedOption = 0;
		int optionCount = 3;
		
		Player player;
		int sleepTime = 0;
		int turnCount = 0;
		int aliensDone = 0;
		int shootingAlien = 0;
		bool alienShot = false;
		
		int spawnX, spawnY;
		
		
		PlayScene(tplay::Game *game, int lvl);
		void setScenes(tplay::Scene *menuScene, EndScene *endScene);
		
		void update();
		void draw();
		
};

#endif
