#include <SDL2/SDL.h>

struct Cell {
	int x;
	int y;
	int size;
	static SDL_Color alive_color;
	static SDL_Color dead_color;
	static SDL_Color border_color;
	bool alive;
	bool prev_alive;

	Cell(int x, int y, int size, bool alive = false): 
		x(x),
		y(y),
		size(size),
		alive(alive),
		prev_alive(alive) 
	{};

	void draw(SDL_Renderer* renderer);
	void update();
};
