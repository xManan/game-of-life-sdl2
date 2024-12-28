#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Grid.hpp"

#define WIDTH 1280
#define HEIGHT 720
#define CELL_SIZE 10

#define FPS 60
#define FRAME_DELAY 1000/FPS

#define N_EDIT_MODE 4
enum EditMode {
	NONE,
	ADD,
	REMOVE,
	TOGGLE,
};

#define N_FONT 7
enum Font {
	ROBOTO_REMIX_16,
	ROBOTO_REMIX_32,
	ROBOTO_REMIX_64,

	STACKER_16,
	STACKER_32,
	STACKER_64,
	STACKER_128,
};

class GameOfLife {
	SDL_Window* window;
	SDL_Renderer* renderer;

	TTF_Font* fonts[N_FONT];

	Grid grid;

	bool running;
	bool paused;
	unsigned int mode;

	Uint32 drawFrameStart;
	Uint32 updateFrameStart;

	SDL_Event event;
	
	void draw();
	void handleInput();
	void update();

	public:
	GameOfLife();
	void gameloop();
	~GameOfLife();
};
