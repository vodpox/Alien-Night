#include "Enemy.hpp"
#include <cmath>
#include <vector>
#include "stdio.h"

Enemy::Enemy(int x, int y, bool targetPlayer) {
	this->x = x;
	this->y = y;
	this->targetPlayer = targetPlayer;
}


int Enemy::getX() {
	return x;
}


int Enemy::getY() {
	return y;
}


int Enemy::getHealth() {
	return health;
}


void Enemy::changeHealth(int change) {
	health += change;
}


void Enemy::changeXY(int x, int y) {
	this->x += x;
	this->y += y;
}


void Enemy::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}


struct Node {
	int x, y;
	bool passable;
	int h, g, f;
	bool open = false, closed = false;
	Node *parent;
};


struct Coords {
	int x, y;
	Coords(int initX, int initY) : x(initX), y(initY) {}
};


void Enemy::update(int spawnX, int spawnY, int playerX, int playerY, int *goX, int *goY, bool *shoot, char Map[1000][1000], int mapX, int mapY) {
	bool doShoot = true;
	
	for (int animationProgress = 1; ; animationProgress++) {
		if (std::abs(x - playerX) + std::abs(y - playerY) > range) {
			doShoot = false;
			break;
		}
		
		double k;
		double lineX;
		double lineY;
		
		if (std::abs( double(playerX) - x ) / std::abs(double(playerY) - y) < 1) { // go y everytime
			k = std::abs( double(playerX) - x ) / std::abs(double(playerY) - y);
			
			if (playerX > x) {
				lineX = k;
			}
			else {
				lineX = k * -1;
			}
			if (playerY > y) {
				lineY = 1;
			}
			else {
				lineY = -1;
			}
		}
		else { // go x everytime
			k = std::abs( double(playerY) - y ) / std::abs(double(playerX) - x);
			
			if (playerX > x) {
				lineX = 1;
			}
			else {
				lineX = -1;
			}
			if (playerY > y) {
				lineY = k;
			}
			else {
				lineY = k * -1;
			}
		}
		
		if (int(x + lineX * animationProgress) < 0 || int(x + lineX * animationProgress) >= mapX || int(y + lineY * animationProgress) < 0 || int(y + lineY * animationProgress) >= mapY) {
			//doShoot = false;
			continue;
		}
		else if (playerX == int(x + lineX * animationProgress) && int(y + lineY * animationProgress) == playerY) {
			doShoot = true;
			break;
		}
		else if (Map[int(x + lineX * animationProgress)][int(y + lineY * animationProgress)] == '#') {
			doShoot = false;
			break;
		}
	}
	
	if (doShoot) { // just why
		*shoot = true;
	}
	else {
		y--; // WHY DO I HAVE TO DO THIS
		*shoot = false;
		*goX = 0;
		*goY = 0;
		
		/* - pathfinding pseudo code
			
			init nodes (heuristics and all that (nope, no time for that) )
			
			add first node to open
			
			while (not done) {
				take from open
				
				if this is the target finish:
					trace back until parent is start
				
				else look around:
					ignore closed
					recheck open
					add nonopen/nonclosed
				
			}
			
		*/
		
		// node init
		Node Nodes[mapX][mapY];
		for (int i = 0; i < mapX; i++) {
			for (int j= 0; j < mapY; j++) {
				Nodes[i][j].x = i;
				Nodes[i][j].y = j;
				Nodes[i][j].passable = (Map[i][j] != '#');
				Nodes[i][j].h = std::abs(x - spawnX) + std::abs(y - spawnY);
			}
		}
		
		// list init
		std::vector<Coords> OpenList;
		OpenList.push_back(Coords(x, y));
		
		// loop de loop
		while (OpenList.size() > 0) {
			
			// if this is the target
			if (OpenList[0].x == spawnX && OpenList[0].y == spawnY) {
				
				Node traceNode = Nodes[OpenList[0].x][OpenList[0].y];
				
				if (traceNode.x == x && traceNode.y == y) {
					*goX = traceNode.x - x;
					*goY = traceNode.y - y;
				}
				
				while (!(traceNode.parent->x == x && traceNode.parent->y == y)) {
					traceNode = *traceNode.parent;
				}
				*goX = traceNode.x - x;
				*goY = traceNode.y - y;
			}
			else {
				// look around the world (or node):
				
				// look up (y++)
				if (OpenList[0].x >= 0 && OpenList[0].x < mapX && OpenList[0].y + 1 >= 0 && OpenList[0].y + 1 < mapY) {
					if (Nodes[OpenList[0].x][OpenList[0].y + 1].closed || !Nodes[OpenList[0].x][OpenList[0].y + 1].passable); // ignore
					else if (Nodes[OpenList[0].x][OpenList[0].y + 1].open) { // special check
						// TODO
					}
					else { // add to open
						Nodes[OpenList[0].x][OpenList[0].y + 1].parent = &Nodes[OpenList[0].x][OpenList[0].y];
						Nodes[OpenList[0].x][OpenList[0].y + 1].open = true;
						OpenList.push_back(Coords(OpenList[0].x, OpenList[0].y + 1));
					}
					Nodes[OpenList[0].x][OpenList[0].y].closed = true;
				}
				
				// look down (y--)
				if (OpenList[0].x >= 0 && OpenList[0].x < mapX && OpenList[0].y - 1 >= 0 && OpenList[0].y - 1 < mapY) {
					if (Nodes[OpenList[0].x][OpenList[0].y - 1].closed || !Nodes[OpenList[0].x][OpenList[0].y + 1].passable); // ignore
					else if (Nodes[OpenList[0].x][OpenList[0].y - 1].open) { // special check
						// TODO
					}
					else { // add to open
						Nodes[OpenList[0].x][OpenList[0].y - 1].parent = &Nodes[OpenList[0].x][OpenList[0].y];
						Nodes[OpenList[0].x][OpenList[0].y - 1].open = true;
						OpenList.push_back(Coords(OpenList[0].x, OpenList[0].y - 1));
					}
					Nodes[OpenList[0].x][OpenList[0].y].closed = true;
				}
				
				// look left (x--)
				if (OpenList[0].x - 1 >= 0 && OpenList[0].x - 1 < mapX && OpenList[0].y >= 0 && OpenList[0].y < mapY) {
					if (Nodes[OpenList[0].x - 1][OpenList[0].y].closed || !Nodes[OpenList[0].x][OpenList[0].y + 1].passable); // ignore
					else if (Nodes[OpenList[0].x - 1][OpenList[0].y].open) { // special check
						// TODO
					}
					else { // add to open
						Nodes[OpenList[0].x - 1][OpenList[0].y].parent = &Nodes[OpenList[0].x][OpenList[0].y];
						Nodes[OpenList[0].x - 1][OpenList[0].y].open = true;
						OpenList.push_back(Coords(OpenList[0].x - 1, OpenList[0].y));
					}
					Nodes[OpenList[0].x][OpenList[0].y].closed = true;
				}
				
				// look right (x++)
				if (OpenList[0].x + 1 >= 0 && OpenList[0].x + 1 < mapX && OpenList[0].y >= 0 && OpenList[0].y < mapY) {
					if (Nodes[OpenList[0].x + 1][OpenList[0].y].closed || !Nodes[OpenList[0].x][OpenList[0].y + 1].passable); // ignore
					else if (Nodes[OpenList[0].x + 1][OpenList[0].y].open) { // special check
						// TODO
					}
					else { // add to open
						Nodes[OpenList[0].x + 1][OpenList[0].y].parent = &Nodes[OpenList[0].x][OpenList[0].y];
						Nodes[OpenList[0].x + 1][OpenList[0].y].open = true;
						OpenList.push_back(Coords(OpenList[0].x + 1, OpenList[0].y));
					}
					Nodes[OpenList[0].x][OpenList[0].y].closed = true;
				}
				
			}
			OpenList.erase(OpenList.begin());
		}
		y++; // D:
	}
}
