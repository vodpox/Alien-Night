#include "Game.h"
#include "Input.h"
#include "TutorialScene.hpp"
#include "cstdio"

TutorialScene::TutorialScene(tplay::Game *game) : game(game) {
	this->game = game;
	this->game->graphics.setCamera(&camera);
}


void TutorialScene::setScenes(tplay::Scene *menu) {
	this->menuScene = menuScene;
}


void TutorialScene::update() {
	
}

void TutorialScene::draw() {
	
}