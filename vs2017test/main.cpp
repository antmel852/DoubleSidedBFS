#include "glut.h"
#include "Cell.h"
#include <time.h>
#include <vector>
#include <iostream>
using namespace std;

const int W = 600;
const int H = 600;
const int WALL = 1;
const int SPACE = 0;
const int START = 2;
const int TARGET = 3;
const int EMARKED = 4; // Cells marked (visited) by target BFS.
const int SMARKED = 5; // Cells marked (visited) by start BFS.
const int PATH = 6; // The shortest path between the points (START and TARGET).

const int MSZ = 100;
int TARGETX = 0, TARGETY = 0;

int maze[MSZ][MSZ] = { 0 };

bool runBFS = false;

vector <Cell*> sCells; // The vector holding all of the cells from the starting point.
vector <Cell*> eCells; // The vector holding all of the cells from the ending point.

void initMaze();

void init()
{
	srand(time(0));
	glClearColor(1, 1, 1, 0);// color of window background
	glOrtho(-1, 1, -1, 1, 0, 1);

	initMaze();
}

void initMaze()
{
	int i, j, endX, endY;

	for (i = 0; i < MSZ; i++) // frame of WALLS
	{
		maze[0][i] = WALL;
		maze[MSZ - 1][i] = WALL;
		maze[i][0] = WALL;
		maze[i][MSZ - 1] = WALL;
	}

	for (i =1; i < MSZ-1; i++)
		for (j = 1; j < MSZ - 1; j++)
		{
			if (i % 2 == 1)
			{
				if (rand() % 100 > 20)
					maze[i][j] = SPACE;
				else
					maze[i][j] = WALL;
			}
			else // the line is even
			{
				if (rand() % 100 > 35)
					maze[i][j] = WALL ;
				else
					maze[i][j] = SPACE;
			}
		}

	maze[MSZ / 2][MSZ / 2] = START;
	endX = rand() % MSZ; // Save ending X point of target.
	endY = rand() % MSZ; // Save ending Y Point of target.
	maze[endX][endY] = TARGET;
	TARGETX = endX;
	TARGETY = endY;

	Cell* sCell = new Cell(MSZ / 2, MSZ / 2 , 0, nullptr); // Create starting point cell. 
	Cell* eCell = new Cell(endX, endY, 1, nullptr); // Create ending point cell.
	sCells.push_back(sCell);
	eCells.push_back(eCell);
}

void drawMaze()
{
	int i, j;
	int xsz = W / MSZ;
	int ysz = H / MSZ;
	double x, y;
	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (maze[i][j])   // set color
			{
			case SPACE:
				glColor3d(0.9, 0.9, 0.9);
				break;
			case WALL:
				glColor3d(0.3, 0, 0);
				break;
			case START:
				glColor3d(0.5, 0.5, 1);
				break;
			case TARGET:
				glColor3d(1, 0, 0);
				break;
			case EMARKED: // Color cells visited by BFS from ending point.
				glColor3d(0.3, 0.9, 0.5);
				break;
			case SMARKED: // Color cells visited by BFS from starting point.
				glColor3d(0.9, 0.9, 0.5);	
				break;
			case PATH: // Color the path found as a solution.
				glColor3d(1, 0.2, 1);
				break;
			}
			// draw square
			x = 2.0 * (j * xsz) / W - 1; // value in range [-1,1)
			y = 2.0 * (i * ysz) / H - 1; // value in range [-1,1)
			glBegin(GL_POLYGON);
			glVertex2d(x, y);
			glVertex2d(x, y+(2.0*ysz)/H);
			glVertex2d(x+2.0*xsz/W, y + (2.0 * ysz) / H);
			glVertex2d(x + 2.0 * xsz / W, y);
			glEnd();
		}
}

void resetMaze() 
{
	for (int i = 0; i < MSZ; i++) 
	{
		for (int j = 0; j < MSZ; j++) 
		{
			if (maze[i][j] == PATH || maze[i][j] == EMARKED || maze[i][j] == SMARKED)
				maze[i][j] = SPACE;
			Cell* sCell = new Cell(MSZ / 2, MSZ / 2, 0, nullptr); // Create starting point cell.
			Cell* eCell = new Cell(TARGETX, TARGETY, 1, nullptr); // Create ending point cell.
			sCells.clear();
			eCells.clear();
			sCells.push_back(sCell);
			eCells.push_back(eCell);
		}
	}
}

// A method that marks the path found as a solution from both BFS's meeting point.
void restorePath(int xPos, int yPos, Cell* cell)
{
	Cell* check; // The second (other than checked at the moment) BFS path meeting cell.

	// Mark path if found via starting point.
	if (cell->getDirection() == 0)
	{
		check = eCells.front();
		eCells.erase(eCells.begin());
		// Find the meeting cell.
		while (check->getXPos() != xPos || check->getYPos() != yPos) {
			check = eCells.front();
			eCells.erase(eCells.begin());
		}
		// Restore Path.
		while (maze[cell->getXPos()][cell->getYPos()] != START)
		{
			maze[cell->getXPos()][cell->getYPos()] = PATH;
			cell = cell->getParent();
		}
		while (maze[check->getXPos()][check->getYPos()] != TARGET)
		{
			maze[check->getXPos()][check->getYPos()] = PATH;
			check = check->getParent();
		}
	}
	// Mark path if found via ending point.
	else
	{
		check = sCells.front();
		sCells.erase(sCells.begin());
		// Find the meeting cell.
		while (check->getXPos() != xPos || check->getYPos() != yPos) {
			check = sCells.front();
			sCells.erase(sCells.begin());
		}
		// Restore Path.
		while (maze[cell->getXPos()][cell->getYPos()] != TARGET)
		{
			maze[cell->getXPos()][cell->getYPos()] = PATH;
			cell = cell->getParent();
		}
		while (maze[check->getXPos()][check->getYPos()] != START)
		{
			maze[check->getXPos()][check->getYPos()] = PATH;
			check = check->getParent();
		}
	}
}

// Checks the neighbor status of a cell at a given point and change it accordingly.
void checkNeighbor(Cell* cell, int xPos, int yPos)
{
	if (maze[xPos][yPos] == SPACE)
	{
		Cell* newCell = new Cell(xPos, yPos, cell->getDirection(), cell);
		if (cell->getDirection() == 0)
		{
			sCells.push_back(newCell);
			maze[xPos][yPos] = SMARKED;
		}
		else
		{
			eCells.push_back(newCell);
			maze[xPos][yPos] = EMARKED;
		}
	}
	else if ((maze[xPos][yPos] == EMARKED && cell->getDirection() == 0) || (maze[xPos][yPos] == SMARKED
		&& cell->getDirection() == 1))
	{
		cout << "Solution Found!";
		runBFS = false;
		restorePath(xPos, yPos, cell);
	}
}

// Check all neighbors of a given cell.
void checkNeighbors(Cell* cell)
{
	checkNeighbor(cell, cell->getXPos() + 1, cell->getYPos()); // Down
	if (runBFS)
		checkNeighbor(cell, cell->getXPos() - 1, cell->getYPos()); // Up
	if (runBFS)
		checkNeighbor(cell, cell->getXPos(), cell->getYPos() + 1); // Right
	if (runBFS)
		checkNeighbor(cell, cell->getXPos(), cell->getYPos() - 1); // Left
}

// Run one iteration of BFS from both sides.
void BFSIteration()
{
	Cell* pCurrentStart; // Current cell from starting point.
	Cell* pCurrentEnd; // Current cell from target point.
	if (sCells.empty() || eCells.empty())
	{
		cout << "There is no solution!";
		runBFS = false;
		return;
	}
	pCurrentStart = sCells.front();
	sCells.erase(sCells.begin());
	pCurrentEnd = eCells.front();
	eCells.erase(eCells.begin());
	if (maze[pCurrentStart->getXPos()][pCurrentStart->getYPos()] != START)
	{
		maze[pCurrentStart->getXPos()][pCurrentStart->getYPos()] = SMARKED;
	}
	if (maze[pCurrentEnd->getXPos()][pCurrentEnd->getYPos()] != TARGET)
	{
		maze[pCurrentEnd->getXPos()][pCurrentEnd->getYPos()] = EMARKED;
	}
	checkNeighbors(pCurrentStart); // Check neighbors of cell from starting point.
	if (runBFS)
		checkNeighbors(pCurrentEnd); // Check neighbors of cell from target point.
}

void bestFirstSearchIteration() 
{

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	drawMaze();
	glutSwapBuffers(); // show all
}

void idle()
{
	if (runBFS)
		BFSIteration();
	glutPostRedisplay(); // go to display
}

void menu(int choice)
{
	switch (choice)
	{
	case 0: // BFS
		runBFS = true;
		break;
	case 1:
		resetMaze();
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("First Example");

	glutDisplayFunc(display); // display is window refresh function
	glutIdleFunc(idle);  // idle is background function

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("BFS", 0);
	glutAddMenuEntry("Reset Maze", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}