#include "Game.h"
#include "Input.h"
#include "EndScene.hpp"
#include "PlayScene.hpp"
#include "cstdio"
#include <unistd.h>
#include <string>

EndScene::EndScene(tplay::Game *game) : game(game) {
	this->game = game;
	this->game->graphics.setCamera(&camera);
}


void EndScene::setScenes(tplay::Scene *menuScene) {
	this->menuScene = menuScene;
}


void EndScene::setData(bool hasWon, int score) {
	this->hasWon = hasWon;
	this->score = score;
	checkHisghscore();
}


int EndScene::getHisghscore() {
	FILE *filep = fopen(".score", "r");
	if (filep == NULL) return false;
	int highscore = 0;
	fscanf(filep, "%i", &highscore);
	fclose(filep);
	return highscore;
}


void EndScene::checkHisghscore() {
	FILE *filep = fopen(".score", "r");
	if (filep == NULL) return;
	highscore = 0;
	fscanf(filep, "%i", &highscore);
	fclose(filep);
	if (highscore < score && hasWon) {
		filep = fopen(".score", "w");
		if (filep == NULL) return;
		fprintf(filep, "%i", score);
		fclose(filep);
		newHighscore = true;
	}
	else {
		newHighscore = false;
	}
}


void EndScene::update() {
	if (game->input.isButtonDown(tplay::Keyboard::Q) || game->input.isButtonDown(tplay::Keyboard::ESCAPE)) {
		game->setScene(menuScene);
	}
}

void EndScene::draw() {
	//printf("%i\n", game->graphics.getTerminalSizeX());
	int x = game->graphics.getTerminalSizeX();
	int y = game->graphics.getTerminalSizeY();
	
	if (hasWon) {
		game->graphics.addToScreen(x/2 - 8, y - y/4 + 1, "Congratulations!");
		game->graphics.addToScreen(x/2 - 4, y - y/4 - 1, "You won!");
		
		game->graphics.addToScreen(x/2 - 14, y - y/4 - 6, "Current highscore is:      ?");
		game->graphics.addToScreen(x/2 + 14 - std::to_string(highscore).size(), y - y/4 - 6, std::to_string(highscore));
		
		game->graphics.addToScreen(x/2 - 17, y - y/4 - 8, "Your score was:      ?");
		game->graphics.addToScreen(x/2 + 17 - std::to_string(score).size(), y - y/4 - 8, std::to_string(score));
		
		if (newHighscore) {
			game->graphics.addToScreen(x/2 - 17, y - y/4 - 11, "You just set the record!");
		}
	}
	else {
		game->graphics.addToScreen(x/2 - 4, y - y/4, "You lost!");
		
		game->graphics.addToScreen(x/2 - 14, y - y/4 - 6, "Current highscore is:      ?");
		game->graphics.addToScreen(x/2 + 14 - std::to_string(highscore).size(), y - y/4 - 6, std::to_string(highscore));
		
		game->graphics.addToScreen(x/2 - 11, y - y/4 - 8, "Your score was:      ?");
		game->graphics.addToScreen(x/2 + 11 - std::to_string(score).size(), y - y/4 - 8, std::to_string(score));
		
		if (newHighscore) {
			game->graphics.addToScreen(x/2 - 15, y - y/4 - 11, "You would have set the record!");
		}
	}
	
	game->graphics.addToScreen(x/2 - 17, y/3 - 5, "Press Q or ESC to go back to menu");
}