#pragma once

class Cell 
{
private:
	int xPos, yPos, direction; // Direction will be 0 (from starting point) or 1 (from ending point). 
	Cell* parent;

public:
	Cell(int x, int y, int direction, Cell* p);
	int getXPos() { return xPos; };
	int getYPos() { return yPos; };
	int getDirection() { return direction; };
	Cell* getParent() { return parent; };
};
