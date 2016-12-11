#include "Portal.hpp"

Portal::Portal(int x, int y) {
	this->x = x;
	this->y = y;
}


void Portal::changeHealth(int change) {
	health += change;
}


int Portal::getHealth() {
	return health;
}


int Portal::getX() {
	return x;
}


int Portal::getY() {
	return y;
}