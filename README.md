## Double Sided BFS
Using OpenGL and C++ a random maze will be generated and left clicking on the maze window will open the options to use the BFS algorithm or to clear the maze.  
Use the classic Breadth First Search algorithm starting from 2 cells.  
First generate a random maze and make a strating cell in the center of the maze, then generate a random target cell.  
Run BFS from each cell while showing the cells they go through.  
When the search from each cell gets to each other stop the search and restore the path.  
To restore the path start popping cells a vector until a cell with the same coordinates is found.
