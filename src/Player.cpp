#include "Player.hpp"


int Player::getX() {
	return x;
}


int Player::getY() {
	return y;
}


void Player::changeHealth(int change) {
	health += change;
}


int Player::getHealth() {
	return health;
}


void Player::move(int x, int y) {
	this->x += x;
	this->y += y;
}


void Player::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}