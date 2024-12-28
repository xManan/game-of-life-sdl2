#include "Cell.hpp"
#include <vector>
#include <SDL2/SDL.h>

class Grid {
	int rows;
	int cols;
	int cell_size;
	std::vector<Cell> cells;

	public:
	Grid(int rows, int cols, int cell_size);

	int getRows();
	int getCols();

	int index(int x, int y);
	Cell& cellAt(int x, int y);
	int totalAliveNeighbors(int x, int y);
	int totalAliveNeighbors(Cell& cell);

	void reset();

	void draw(SDL_Renderer* renderer);
	void update();
};
