#ifndef PORTAL
#define PORTAL

class Portal {
	private:
		int x, y;
		int health = 75;
	
	public:
		Portal(int x, int y);
		void changeHealth(int change);
		int getHealth();
		int getX();
		int getY();
	
};

#endif
