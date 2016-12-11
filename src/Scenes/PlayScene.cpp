#include "Game.h"
#include "Input.h"
#include "PlayScene.hpp"
#include <cstdio>
#include <string>
#include <cmath>
#include <unistd.h>


void PlayScene::loadMap(int level) {
	this->level = level;
	
	FILE *filep = fopen((std::string("Maps/Level") + std::to_string(level) + std::string(".txt")).c_str(), "r");
	if (filep == NULL) game->setScene(menuScene); // if no level data exists, return to menu
	fscanf(filep, "%i %i\n", &mapX, &mapY);
	
	for (int y = mapY-1; y >= 0; y--) {
		for (int x = 0; x < mapX; x++) {
			Map[x][y] = getc(filep);
			
			if (Map[x][y] == '!') {
				spawnX = x;
				spawnY = y;
				player.setXY(x, y);
				resetAim();
			}
			
			if (Map[x][y] == 'O') {
				Portals.push_back(new Portal(x, y));
			}
			
			if (Map[x][y] == 'A') {
				Enemies.push_back(new Enemy(x, y, true));
			}
		}
		fscanf(filep, "\n");
	}
}


bool PlayScene::collides(int x, int y, bool notOnlyWalls) {
	
	// check map
	if (x < 0 || x >= mapX || y < 0 || y >= mapY) return true;
	if (Map[x][y] == '#') return true;
	
	if (notOnlyWalls) {
		// player
		if (x == player.getX() && y == player.getY()) return true;
		
		//enemies
		for (int i = 0; i < Enemies.size(); i++) {
			if (x == Enemies[i]->getX() && y == Enemies[i]->getY() && Enemies[i]->getHealth() > 0) return true;
		}
		
		//portals
		for (int i = 0; i < Portals.size(); i++) {
			if (x == Portals[i]->getX() && y == Portals[i]->getY() && Portals[i]->getHealth() > 0) return true;
		}
		
		// ???
		
	}
	
	return false;
}


bool PlayScene::inRange(int x1, int y1, int x2, int y2, int range) {
	if (std::abs(std::abs(x1 - x2) + std::abs(y1 - y2)) > range) {
		return false;
	}
	else return true;
}


void PlayScene::resetAim() {
	aimX = player.getX();
	aimY = player.getY();
}


PlayScene::PlayScene(tplay::Game *game, int lvl) : game(game) {
	this->game = game;
	this->game->graphics.setCamera(&camera);
	this->level = lvl;
	
	loadMap(level);
}


void PlayScene::setScenes(tplay::Scene *menuScene, EndScene *endScene) {
	this->menuScene = menuScene;
	this->endScene = endScene;
}


void PlayScene::update() {
	
	if (sleepTime > 500 && game->input.isButtonDown(tplay::Keyboard::ENTER)) sleepTime = 50;
	
	usleep(sleepTime);
	sleepTime = 0;
	
	if (alienShot) {
		animationProgress++;
		
		double k;
		double lineX;
		double lineY;
		
		if (std::abs( double(player.getX()) - Enemies[shootingAlien]->getX() ) / std::abs(double(player.getY()) - Enemies[shootingAlien]->getY()) < 1) { // go y everytime
			k = std::abs( double(player.getX()) - Enemies[shootingAlien]->getX() ) / std::abs(double(player.getY()) - Enemies[shootingAlien]->getY());
			
			if (player.getX() > Enemies[shootingAlien]->getX()) {
				lineX = k;
			}
			else {
				lineX = k * -1;
			}
			if (player.getY() > Enemies[shootingAlien]->getY()) {
				lineY = 1;
			}
			else {
				lineY = -1;
			}
		}
		else { // go x everytime
			k = std::abs( double(player.getY()) - Enemies[shootingAlien]->getY() ) / std::abs(double(player.getX()) - Enemies[shootingAlien]->getX());
			
			if (player.getX() > Enemies[shootingAlien]->getX()) {
				lineX = 1;
			}
			else {
				lineX = -1;
			}
			if (player.getY() > Enemies[shootingAlien]->getY()) {
				lineY = k;
			}
			else {
				lineY = k * -1;
			}
		}
		
		if (Enemies[shootingAlien]->getX() + lineX * animationProgress == player.getX() && Enemies[shootingAlien]->getY() + lineY * animationProgress == player.getY() || collides(Enemies[shootingAlien]->getX() + lineX * animationProgress, Enemies[shootingAlien]->getY() + lineY * animationProgress, false)) {
			
			if (player.getX() == Enemies[shootingAlien]->getX() + lineX * animationProgress && player.getY() == Enemies[shootingAlien]->getY() + lineY * animationProgress ) player.changeHealth(-5);
			
			animationProgress = 0;
			animationStage = 0;
			alienShot = false;
		}
		
	}
	else if (shootingAnimation) {
		animationProgress++;
		
		// line thing
		double k;
		double lineX;
		double lineY;
		
		if (std::abs( double(aimX) - player.getX() ) / std::abs(double(aimY) - player.getY()) < 1) { // go y everytime
			k = std::abs( double(aimX) - player.getX() ) / std::abs(double(aimY) - player.getY());
			
			if (aimX > player.getX()) {
				lineX = k;
			}
			else {
				lineX = k * -1;
			}
			if (aimY > player.getY()) {
				lineY = 1;
			}
			else {
				lineY = -1;
			}
		}
		else { // go x everytime
			k = std::abs( double(aimY) - player.getY() ) / std::abs(double(aimX) - player.getX());
			
			if (aimX > player.getX()) {
				lineX = 1;
			}
			else {
				lineX = -1;
			}
			if (aimY > player.getY()) {
				lineY = k;
			}
			else {
				lineY = k * -1;
			}
		}
		
		switch (currentWeapon) {
			case 0: // pistol
				if (collides(player.getX() + lineX * animationProgress, player.getY() + lineY * animationProgress, true) || (player.getX() + lineX * animationProgress == aimX && player.getY() + lineY * animationProgress == aimY) ) {
					
					for (int i = 0; i < Enemies.size(); i++) {
						if (Enemies[i]->getX() == player.getX() + lineX * animationProgress && Enemies[i]->getY() == player.getY() + lineY * animationProgress ) {
							Enemies[i]->changeHealth(-100);
							score += 250;
						}
					}
					
					for (int i = 0; i < Portals.size(); i++) {
						if (Portals[i]->getX() == player.getX() + lineX * animationProgress && Portals[i]->getY() == player.getY() + lineY * animationProgress ) {
							Portals[i]->changeHealth(-100);
							score += 250;
						}
					}
					
					animationProgress = 0;
					animationStage = 0;
					shootingAnimation = false;
				}
				break;
			
			case 1: // laser
				
				for (int i = 0; i < Enemies.size(); i++) {
					if (Enemies[i]->getX() == player.getX() + lineX * animationProgress && Enemies[i]->getY() == player.getY() + lineY * animationProgress ) {
						Enemies[i]->changeHealth(-100);
						score += 750;
					}
				}
				
				for (int i = 0; i < Portals.size(); i++) {
					if (Portals[i]->getX() == player.getX() + lineX * animationProgress && Portals[i]->getY() == player.getY() + lineY * animationProgress ) {
						Portals[i]->changeHealth(-100);
						score += 750;
					}
				}
				
				if (collides(player.getX() + lineX * animationProgress, player.getY() + lineY * animationProgress, false) || !inRange(player.getX() + lineX * animationProgress, player.getY() + lineY * animationProgress, aimX, aimY, WeaponRange[1])) {
					animationProgress = 0;
					animationStage = 0;
					shootingAnimation = false;
				}
				break;
			
			case 2: // grenade
				if (animationStage == 1) {
					/*for (int y = -1 - grenadeRange; y <= grenadeRange; y++) {
						for (int x = std::abs(y) - grenadeRange; x <= grenadeRange - std::abs(y); x++) {
							
							for (int i = 0; i < Enemies.size(); i++) {
								if (aimX + Enemies[i]->getX() == x && aimY + Enemies[i]->getY() == y ) Enemies[i]->changeHealth(-100);
							}
							
						}
					}*/
					
					for (int i = 0; i < Enemies.size(); i++) {
						if (inRange(Enemies[i]->getX(), Enemies[i]->getY(), aimX, aimY, grenadeRange)) {
							Enemies[i]->changeHealth(-100);
							score += 1000;
						}
					}
					
					for (int i = 0; i < Portals.size(); i++) {
						if (inRange(Portals[i]->getX(), Portals[i]->getY(), aimX, aimY, grenadeRange)) {
							Portals[i]->changeHealth(-100);
							score += 1000;
						}
					}
					
					if (animationProgress > 0) {
						animationProgress = 0;
						animationStage = 0;
						shootingAnimation = false;
					}
				}
				else {
					if (player.getX() + lineX * animationProgress == aimX && player.getY() + lineY * animationProgress == aimY) {
						animationStage = 1;
						animationProgress = 0;
					}
				}
				break;
			
		}
		
	}
	else {
		if (!endTurn) {
			// pause/quit
			if (game->input.isButtonDown(tplay::Keyboard::ESCAPE)) {
				game->setScene(menuScene);
			}
			
			// movement
			if (game->input.isButtonDown(tplay::Keyboard::W)) {
				if (aiming) {
					aimY++;
				}
				else {
					if (!collides(player.getX(), player.getY() + 1, true)) {
						player.move(0, 1);
						resetAim();
						endTurn = true;
					}
				}
			}
			if (game->input.isButtonDown(tplay::Keyboard::A)) {
				if (aiming) {
					aimX--;
				}
				else {
					if (!collides(player.getX() - 1, player.getY(), true)) {
						player.move(-1, 0);
						resetAim();
						endTurn = true;
					}
				}
			}
			if (game->input.isButtonDown(tplay::Keyboard::S)) {
				if (aiming) {
					aimY--;
				}
				else {
					if (!collides(player.getX(), player.getY() - 1, true)) {
						player.move(0, -1);
						resetAim();
						endTurn = true;
					}
				}
			}
			if (game->input.isButtonDown(tplay::Keyboard::D)) {
				if (aiming) {
					aimX++;
				}
				else {
					if (!collides(player.getX() + 1, player.getY(), true)) {
						player.move(1, 0);
						resetAim();
						endTurn = true;
					}
				}
			}
			if (game->input.isButtonDown(tplay::Keyboard::ENTER)) {
				endTurn = true;
			}
			
			// fighting
			if (game->input.isButtonDown(tplay::Keyboard::SPACEBAR)) {
				aiming = !aiming;
				//resetAim();
			}
			if (game->input.isButtonDown(tplay::Keyboard::TAB) && aiming) {
				currentWeapon++;
				if (currentWeapon >= weaponCount) currentWeapon = 0;
			}
			if (game->input.isButtonDown(tplay::Keyboard::ENTER) && aiming) {
				if (inRange(player.getX(), player.getY(), aimX, aimY, WeaponRange[currentWeapon])) {
					switch (currentWeapon) {
						case 0: // pistol
							if (true) { // TODO: (don't?) limit ammo
								aiming = false;
								shootingAnimation = true;
								endTurn = true;
								return;
							}
							break;
						
						case 1: // laser
							if (true) { // TODO: limit ammo
								aiming = false;
								shootingAnimation = true;
								endTurn = true;
								return;
							}
							break;
						
						case 2: // grenade
							if (true) { // TODO: limit ammo
								aiming = false;
								shootingAnimation = true;
								endTurn = true;
								return;
							}
							break;
						
					}
				}
			}
		}
		else{
			
			// Enemies do stuff here!
			for (int i = aliensDone; i < Enemies.size(); i++) {
				aliensDone++;
				if (Enemies[i]->getHealth() <= 0) {
					return;
				}
				
				int tryX, tryY;
				bool doesShoot;
				Enemies[i]->update(spawnX, spawnY, player.getX(), player.getY(), &tryX, &tryY, &doesShoot, Map, mapX, mapY);
				
				if (doesShoot) {
					alienShot = true;
					shootingAlien = i;
					return;
				}
				else if (!collides(Enemies[i]->getX() + tryX, Enemies[i]->getY() + tryY, true) && Enemies[i]->getHealth() > 0) {
					Enemies[i]->changeXY(tryX, tryY);
					return;
				}
			}
			aliensDone = 0;
			endTurn = false;
			
			// end turn things
			
			// if lost
			if (player.getHealth() <= 0) {
				endScene->setData(false, score);
				game->setScene(endScene);
			}
			
			for (int i = 0; i < Enemies.size(); i++) {
				if (std::abs(Enemies[i]->getX() - spawnX) + std::abs(Enemies[i]->getY() - spawnY) <= 1) {
					endScene->setData(false, score);
					game->setScene(endScene);
				}
			}
			
			// if won
			if (Portals.size() <= 0) {
				
				if (level == 3) {
					endScene->setData(true, score);
					game->setScene(endScene);
				}
				else {
					PlayScene *newLevel = new PlayScene(game, level + 1);
					newLevel->setScenes(menuScene, endScene);
					game->setScene(newLevel);
				}
			}
			
			if (score > 0) score -= 10;
			
			for (int i = 0; i < Portals.size(); i++) {
				if (turnCount % 25 == 0) {
					Enemies.push_back(new Enemy(Portals[i]->getX(), Portals[i]->getY(), true));
				}
			}
			turnCount++;
			
			if (player.getX() == spawnX && player.getY() == spawnY) player.changeHealth(5);
		}
	}
	
	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i]->getHealth() < -75) {
			Enemies.erase(Enemies.begin() + i);
			i--;
		}
	}
	
	for (int i = 0; i < Portals.size(); i++) {
		if (Portals[i]->getHealth() <= 0) {
			Portals.erase(Portals.begin() + i);
			i--;
		}
	}
}


void PlayScene::draw() {
	int termX = game->graphics.getTerminalSizeX();
	int termY = game->graphics.getTerminalSizeY();
	
	if (aiming) {
		camera.setCoordinates(aimX - game->graphics.getTerminalSizeX() / 2, aimY - game->graphics.getTerminalSizeY() / 2);
		// aiming range
		for (int y = -1 - WeaponRange[currentWeapon]; y <= WeaponRange[currentWeapon]; y++) {
			for (int x = std::abs(y) - WeaponRange[currentWeapon]; x <= WeaponRange[currentWeapon] - std::abs(y); x++) {
				if (!(x + player.getX() < 0 || x + player.getX() >= mapX || y + player.getY() < 0 || y + player.getY() >= mapY)) game->graphics.addToWorld(player.getX() + x, player.getY() + y, ".");
			}
		}
	}
	else {
		camera.setCoordinates(player.getX() - game->graphics.getTerminalSizeX() / 2, player.getY() - game->graphics.getTerminalSizeY() / 2);
	}
	
	// map
	for (int y = 0; y < mapY; y++) {
		for (int x = 0; x < mapX; x++) {
			if (Map[x][y] == '#') game->graphics.addToWorld(x, y, "#");
		}
	}
	// dead enemies
	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i]->getHealth() > 50);
		else if (Enemies[i]->getHealth() > 0);
		else if (Enemies[i]->getHealth() > -75) game->graphics.addToWorld(Enemies[i]->getX(), Enemies[i]->getY(), "_");
	}
	
	// spawn
	game->graphics.addToWorld(spawnX, spawnY, "!");
	
	// Portals
	for (int i = 0; i < Portals.size(); i++) {
		game->graphics.addToWorld(Portals[i]->getX(), Portals[i]->getY(), "O");
	}
	
	// player
	game->graphics.addToWorld(player.getX(), player.getY(), "@");
	
	// enemies
	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i]->getHealth() > 50);
		else if (Enemies[i]->getHealth() > 0) game->graphics.addToWorld(Enemies[i]->getX(), Enemies[i]->getY(), "a");
		else if (Enemies[i]->getHealth() > -75);
	}
	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i]->getHealth() > 50) game->graphics.addToWorld(Enemies[i]->getX(), Enemies[i]->getY(), "A");
		else if (Enemies[i]->getHealth() > 0);
		else if (Enemies[i]->getHealth() > -75);
	}
	
	// shooting
	if (alienShot) {
		double k;
		double lineX;
		double lineY;
		
		if (std::abs( double(player.getX()) - Enemies[shootingAlien]->getX() ) / std::abs(double(player.getY()) - Enemies[shootingAlien]->getY()) < 1) { // go y everytime
			k = std::abs( double(player.getX()) - Enemies[shootingAlien]->getX() ) / std::abs(double(player.getY()) - Enemies[shootingAlien]->getY());
			
			if (player.getX() > Enemies[shootingAlien]->getX()) {
				lineX = k;
			}
			else {
				lineX = k * -1;
			}
			if (player.getY() > Enemies[shootingAlien]->getY()) {
				lineY = 1;
			}
			else {
				lineY = -1;
			}
		}
		else { // go x everytime
			k = std::abs( double(player.getY()) - Enemies[shootingAlien]->getY() ) / std::abs(double(player.getX()) - Enemies[shootingAlien]->getX());
			
			if (player.getX() > Enemies[shootingAlien]->getX()) {
				lineX = 1;
			}
			else {
				lineX = -1;
			}
			if (player.getY() > Enemies[shootingAlien]->getY()) {
				lineY = k;
			}
			else {
				lineY = k * -1;
			}
		}
		
		for (int i = animationProgress-2; i < animationProgress; i++) {
			if (i > 0) game->graphics.addToWorld(Enemies[shootingAlien]->getX() + lineX * i, Enemies[shootingAlien]->getY() + lineY * i, ".");
		}
		sleepTime += 30000;
	}
	
	if (shootingAnimation) {
		
		double k;
		double lineX;
		double lineY;
		
		if (std::abs( double(aimX) - player.getX() ) / std::abs(double(aimY) - player.getY()) < 1) { // go y everytime
			k = std::abs( double(aimX) - player.getX() ) / std::abs(double(aimY) - player.getY());
			
			if (aimX > player.getX()) {
				lineX = k;
			}
			else {
				lineX = k * -1;
			}
			if (aimY > player.getY()) {
				lineY = 1;
			}
			else {
				lineY = -1;
			}
		}
		else { // go x everytime
			k = std::abs( double(aimY) - player.getY() ) / std::abs(double(aimX) - player.getX());
			
			if (aimX > player.getX()) {
				lineX = 1;
			}
			else {
				lineX = -1;
			}
			if (aimY > player.getY()) {
				lineY = k;
			}
			else {
				lineY = k * -1;
			}
		}
		
		switch (currentWeapon) {
			case 0: // pistol
				
				for (int i = animationProgress-5; i < animationProgress; i++) {
					if (i > 0) game->graphics.addToWorld(player.getX() + lineX * i, player.getY() + lineY * i, ".");
				}
				sleepTime += 50000;
				
				break;
			
			case 1: // laser
				
				for (int i = 1; i < animationProgress; i++) {
					game->graphics.addToWorld(player.getX() + lineX * i, player.getY() + lineY * i, "*");
				}
				sleepTime += 50000;
				
				break;
			
			case 2: // grenade
				if (animationStage == 1) { // explosion
					for (int y = -1 - grenadeRange; y <= grenadeRange; y++) {
						for (int x = std::abs(y) - grenadeRange; x <= grenadeRange - std::abs(y); x++) {
							game->graphics.addToWorld(aimX + x, aimY + y, "^");
						}
					}
					sleepTime += 200000;
				}
				else { // throwing
					game->graphics.addToWorld(player.getX() + lineX * animationProgress, player.getY() + lineY * animationProgress, "o");
					sleepTime += 50000;
				}
				break;
		}
	}
	
	// aiming
	if (aiming) {
		// aimming cursor or whatever it's called
		if (inRange(player.getX(), player.getY(), aimX, aimY, WeaponRange[currentWeapon])) {
			game->graphics.addToWorld(aimX, aimY, "X");
		}
		else {
			game->graphics.addToWorld(aimX, aimY, "x");
		}
		// UI - aiming
		game->graphics.setFormat(tplay::Format::NEGATIVE);
		switch (currentWeapon) {
			case 0:
				game->graphics.addToScreen(termX/2 - 15, termY - 1, "Currently aiming with:   rifle");
				break;
			case 1:
				game->graphics.addToScreen(termX/2 - 15, termY - 1, "Currently aiming with:   laser");
				break;
			case 2:
				game->graphics.addToScreen(termX/2 - 15, termY - 1, "Currently aiming with: grenade");
				break;
		}
		if (!inRange(player.getX(), player.getY(), aimX, aimY, WeaponRange[currentWeapon])) game->graphics.addToScreen(termX/2 - 6, termY - 5, "NOT IN RANGE");
		game->graphics.unsetFormat(tplay::Format::NEGATIVE);
	}
	
	// UI - stats
	game->graphics.setFormat(tplay::Format::NEGATIVE);
	
	game->graphics.addToScreen(0, termY - 1, " Score:        ");
	game->graphics.addToScreen(14 - std::to_string(score).size(), termY - 1, std::to_string(score));
	
	game->graphics.addToScreen(termX - 10, termY - 1, " Level:   ");
	game->graphics.addToScreen(termX - 1 - std::to_string(level).size(), termY - 1, std::to_string(level));
	
	game->graphics.addToScreen(0, 0, " Rifle:  UNLIMITED ");
	
	game->graphics.addToScreen(22, 0, " Laser:  UNLIMITED ");
	
	game->graphics.addToScreen(44, 0, " Grenades: UNLIMITED ");
	
	game->graphics.addToScreen(termX - 13, 0, " Health:     ");
	game->graphics.addToScreen(termX - 1 - std::to_string(player.getHealth()).size(), 0, std::to_string(player.getHealth()));
	
	game->graphics.unsetFormat(tplay::Format::NEGATIVE);
	
	// UI - pause menu
	// nope :c
}
