#include "Grid.hpp"

Grid::Grid(int rows, int cols, int cellSize): rows(rows), cols(cols), cell_size(cellSize) {
	int totalCells = rows * cols;
	for(int i=0; i<rows; ++i) {
		for(int j=0; j<cols; ++j) {
			Cell& cell = cells.emplace_back(j, i, cellSize);
		}
	}
}

int Grid::getRows() {
	return rows;
}

int Grid::getCols() {
	return cols;
}

void Grid::reset() {
	for(Cell& cell: cells) {
		cell.alive = false;
		cell.prev_alive = false;
	}
}

void Grid::draw(SDL_Renderer* renderer) {
	for(Cell& cell: cells) {
		cell.draw(renderer);
	}
}

void Grid::update() {
	for(Cell& cell: cells) {
		if(cell.prev_alive && (totalAliveNeighbors(cell) < 2 || totalAliveNeighbors(cell) > 3)) {
			cell.alive = false;
		} else if(totalAliveNeighbors(cell) == 3) {
			cell.alive = true;
		}
	}
	for(Cell& cell: cells) {
		cell.prev_alive = cell.alive;
	}
}

int Grid::index(int x, int y) {
	return y * cols + x;
}

Cell& Grid::cellAt(int x, int y) {
	return cells.at(index(x, y));
}

int Grid::totalAliveNeighbors(Cell& cell) {
	return totalAliveNeighbors(cell.x, cell.y);
}

int Grid::totalAliveNeighbors(int x, int y) {
	int aliveNeighbours = 0;
	if(x == 0 && y == 0) {
		cellAt(x + 1,     y).prev_alive && ++aliveNeighbours;
		cellAt(x,     y + 1).prev_alive && ++aliveNeighbours;
		cellAt(x + 1, y + 1).prev_alive && ++aliveNeighbours;
	} else if(x == 0 && y == rows - 1) {
		cellAt(x,     y - 1).prev_alive && ++aliveNeighbours;
		cellAt(x + 1,     y).prev_alive && ++aliveNeighbours;
		cellAt(x + 1, y - 1).prev_alive && ++aliveNeighbours;
	} else if(x == cols - 1 && y == 0) {
		cellAt(x - 1,     y).prev_alive && ++aliveNeighbours;
		cellAt(x,     y + 1).prev_alive && ++aliveNeighbours;
		cellAt(x - 1, y + 1).prev_alive && ++aliveNeighbours;
	} else if(x == cols - 1 && y == rows - 1) {
		cellAt(x,     y - 1).prev_alive && ++aliveNeighbours;
		cellAt(x - 1,     y).prev_alive && ++aliveNeighbours;
		cellAt(x - 1, y - 1).prev_alive && ++aliveNeighbours;
	} else if(x == 0) {
		cellAt(x,     y - 1).prev_alive && ++aliveNeighbours;
		cellAt(x + 1, y - 1).prev_alive && ++aliveNeighbours;
		cellAt(x + 1,     y).prev_alive && ++aliveNeighbours;
		cellAt(x + 1, y + 1).prev_alive && ++aliveNeighbours;
		cellAt(x,     y + 1).prev_alive && ++aliveNeighbours;
	} else if(x == cols - 1) {
		cellAt(x,     y - 1).prev_alive && ++aliveNeighbours;
		cellAt(x - 1, y - 1).prev_alive && ++aliveNeighbours;
		cellAt(x - 1,     y).prev_alive && ++aliveNeighbours;
		cellAt(x - 1, y + 1).prev_alive && ++aliveNeighbours;
		cellAt(x,     y + 1).prev_alive && ++aliveNeighbours;
	} else if(y == 0) {
		cellAt(x - 1,     y).prev_alive && ++aliveNeighbours;
		cellAt(x - 1, y + 1).prev_alive && ++aliveNeighbours;
		cellAt(x,     y + 1).prev_alive && ++aliveNeighbours;
		cellAt(x + 1, y + 1).prev_alive && ++aliveNeighbours;
		cellAt(x + 1,     y).prev_alive && ++aliveNeighbours;
	} else if(y == rows - 1) {
		cellAt(x - 1,     y).prev_alive && ++aliveNeighbours;
		cellAt(x - 1, y - 1).prev_alive && ++aliveNeighbours;
		cellAt(x,     y - 1).prev_alive && ++aliveNeighbours;
		cellAt(x + 1, y - 1).prev_alive && ++aliveNeighbours;
		cellAt(x + 1,     y).prev_alive && ++aliveNeighbours;
	} else {
		cellAt(x - 1,     y).prev_alive && ++aliveNeighbours;
		cellAt(x - 1, y + 1).prev_alive && ++aliveNeighbours;
		cellAt(x,     y + 1).prev_alive && ++aliveNeighbours;
		cellAt(x + 1, y + 1).prev_alive && ++aliveNeighbours;
		cellAt(x + 1,     y).prev_alive && ++aliveNeighbours;
		cellAt(x + 1, y - 1).prev_alive && ++aliveNeighbours;
		cellAt(x,     y - 1).prev_alive && ++aliveNeighbours;
		cellAt(x - 1, y - 1).prev_alive && ++aliveNeighbours;
	}
	return aliveNeighbours;
}
