#ifndef ENEMY
#define ENEMY


class Enemy {
	
	private:
		int x, y;
		int health = 100;
		bool targetPlayer;
		int range = 25;
	
	public:
		Enemy(int x, int y, bool targetPlayer);
		int getX();
		int getY();
		int getHealth();
		void changeHealth(int change);
		void changeXY(int x, int y);
		void setXY(int x, int y);
		void update(int spawnX, int spawnY, int playerX, int playerY, int *goX, int *goY, bool *shoot, char Map[1000][1000], int mapX, int mapY);
	
};

#endif
