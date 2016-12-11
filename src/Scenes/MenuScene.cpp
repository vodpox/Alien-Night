#include "Game.h"
#include "Input.h"
#include "MenuScene.hpp"
#include "PlayScene.hpp"
#include "cstdio"

MenuScene::MenuScene(tplay::Game *game) : game(game) {
	this->game = game;
	this->game->graphics.setCamera(&camera);
}


void MenuScene::setScenes(tplay::Scene *tutorialScene, EndScene *endScene) {
	this->tutorialScene = tutorialScene;
	this->endScene = endScene;
}


void MenuScene::update() {
	if (game->input.isButtonDown(tplay::Keyboard::Q) || game->input.isButtonDown(tplay::Keyboard::ESCAPE)) {
		game->quit();
	}
	if (game->input.isButtonDown(tplay::Keyboard::UP_ARROW)) {
		if (selectedOption > 0) selectedOption--;
	}
	if (game->input.isButtonDown(tplay::Keyboard::DOWN_ARROW)) {
		if (selectedOption < optionCount-1) selectedOption++;
	}
	if (game->input.isButtonDown(tplay::Keyboard::ENTER)) {
		switch (selectedOption) {
			case 0:
				{
					playScene = new PlayScene(game, 1);
					playScene->setScenes(this, endScene);
					game->setScene(playScene);
				}
				
				break;
			case 1:
				rip = !rip;
				break;
			case 2:
				game->quit();
				break;
		}
	}
}

void MenuScene::draw() {
	int x = game->graphics.getTerminalSizeX();
	int y = game->graphics.getTerminalSizeY();
	
	game->graphics.setFormat(tplay::Format::NEGATIVE);
	game->graphics.addToScreen(x/2 - 6, y - y/4 - 1, "            ");
	game->graphics.addToScreen(x/2 - 6, y - y/4 - 2, " Aien Night ");
	game->graphics.addToScreen(x/2 - 6, y - y/4 - 3, "            ");
	game->graphics.unsetFormat(tplay::Format::NEGATIVE);
	
	game->graphics.addToScreen(x/2 - 2, y/3 - 0, "Play");
	if (rip) game->graphics.addToScreen(x/2 - 8, y/3 - 2, "Not enough time!");
	else game->graphics.addToScreen(x/2 - 4, y/3 - 2, "Tutorial");
	//game->graphics.addToScreen(x/2 - 2, y/2 - 0, "Highscore");
	game->graphics.addToScreen(x/2 - 2, y/3 - 4, "Exit");
	
	game->graphics.addToScreen(x/2 - 9, y/3 - 2*selectedOption, ">");
	game->graphics.addToScreen(x/2 + 8, y/3 - 2*selectedOption, "<");
}