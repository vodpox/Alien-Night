#include "Game.h"

#include "Scenes/EndScene.hpp"
#include "Scenes/TutorialScene.hpp"
#include "Scenes/MenuScene.hpp"


int main() {
	tplay::Game game;
	MenuScene menuScene(&game);
	TutorialScene tutorialScene(&game);
	EndScene endScene(&game);
	menuScene.setScenes(&tutorialScene, &endScene);
	endScene.setScenes(&menuScene);
	game.setScene(&menuScene);
	game.loop();
	return 0;
}
