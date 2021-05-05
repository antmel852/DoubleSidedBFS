#include "Cell.h"

Cell::Cell(int x, int y, int d, Cell* p)
{
	xPos = x;
	yPos = y;
	direction = d;
	parent = p;
}