#include "GameOfLife.hpp"
#include "Color.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_filesystem.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <cstring>
#include <iostream>

std::string absolutePath(const char* relativePath) {
	char* basePath = SDL_GetBasePath();
	if (basePath == NULL || relativePath == NULL) {
		return std::string();
	}
	std::string filePath = basePath;
	filePath += relativePath;
	SDL_free(basePath);
	return filePath;
}

void SDLHelper_RenderText(SDL_Renderer* renderer, const char* text, 
							TTF_Font* font, const SDL_Color& color, 
							int x = 0, int y = 0) {
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
		if (!textSurface) {
			std::cerr << "Failed to render text surface: " << TTF_GetError() << std::endl;
			exit(1);
		}
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (!textTexture) {
			std::cerr << "Failed to create texture from text surface: " << SDL_GetError() << std::endl;
			SDL_FreeSurface(textSurface);
			exit(1);
		}
		SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
		SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
}

void SDLHelper_RenderTextP(SDL_Renderer* renderer, const char* text, 
							TTF_Font* font, const SDL_Color& color, 
							float x_p = 0, float y_p = 0, 
							float translate_x_p = 0, float translate_y_p = 0) {
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
		if (!textSurface) {
			std::cerr << "Failed to render text surface: " << TTF_GetError() << std::endl;
			exit(1);
		}
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (!textTexture) {
			std::cerr << "Failed to create texture from text surface: " << SDL_GetError() << std::endl;
			SDL_FreeSurface(textSurface);
			exit(1);
		}

		float pos_x = WIDTH * (x_p / 100) + (textSurface->w * (translate_x_p / 100));
		float pos_y = HEIGHT * (y_p / 100) + (textSurface->h * (translate_y_p / 100));

		SDL_FRect renderQuad = { pos_x, pos_y, (float)textSurface->w, (float)textSurface->h };
		SDL_RenderCopyF(renderer, textTexture, nullptr, &renderQuad);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
}


GameOfLife::GameOfLife(): grid(HEIGHT / CELL_SIZE, WIDTH / CELL_SIZE, CELL_SIZE) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL_video initialization failed: " << SDL_GetError() << std::endl;
		exit(1);
	}

	SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
	window = SDL_CreateWindow(
			"Simulation",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WIDTH,
			HEIGHT,
			SDL_WINDOW_SHOWN
			);

	if(window == NULL) {
		std::cout << "SDL failed to create window. SDL error: " << SDL_GetError() << std::endl;
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if(renderer == NULL) {
		std::cout << "SDL failed to create renderer. SDL error: " << SDL_GetError() << std::endl;
		exit(1);
	}

	if(TTF_Init() == -1) {
		std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
		exit(1);
	}
	
	fonts[ROBOTO_REMIX_16] = TTF_OpenFont(absolutePath("../assets/fonts/roboto-remix.ttf").c_str(), 16);
	fonts[ROBOTO_REMIX_32] = TTF_OpenFont(absolutePath("../assets/fonts/roboto-remix.ttf").c_str(), 32);
	fonts[ROBOTO_REMIX_64] = TTF_OpenFont(absolutePath("../assets/fonts/roboto-remix.ttf").c_str(), 64);

	fonts[STACKER_16]  = TTF_OpenFont(absolutePath("../assets/fonts/stacker.ttf").c_str(), 16);
	fonts[STACKER_32]  = TTF_OpenFont(absolutePath("../assets/fonts/stacker.ttf").c_str(), 32);
	fonts[STACKER_64]  = TTF_OpenFont(absolutePath("../assets/fonts/stacker.ttf").c_str(), 64);
	fonts[STACKER_128] = TTF_OpenFont(absolutePath("../assets/fonts/stacker.ttf").c_str(), 128);

	for(TTF_Font* font: fonts) {
		if (!font) {
			std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
			exit(1);
		}
	}

	running = true;
	paused = true;
	mode = EditMode::NONE;
	drawFrameStart = SDL_GetTicks() + FRAME_DELAY;
	updateFrameStart = SDL_GetTicks();
}

void GameOfLife::draw() {
	grid.draw(renderer);
	if(paused) {
		SDLHelper_RenderTextP(renderer, "Conway's", fonts[ROBOTO_REMIX_32], Color::BLACK, 50, 40, -50, -50);
		SDLHelper_RenderTextP(renderer, "GAME OF LIFE", fonts[STACKER_128], Color::BLACK, 50, 50, -50, -50);
	}
	SDL_RenderPresent(renderer);
}

void GameOfLife::handleInput() {
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_SPACE:
						paused = !paused;
						break;
					case SDLK_h:
						// help
						break;
					case SDLK_e:
						if(!paused) {
							paused = true;
							break;
						}
						// if e then next mode else shift + e prev mode
						mode = (mode + ((SDL_GetModState() & KMOD_SHIFT) ? -1 : 1)) % N_EDIT_MODE;
						break;
					case SDLK_r:
						grid.reset();
						break;
					case SDLK_l:
						// list samples
						break;
				}
				if(paused) {
					switch(mode) {
						case EditMode::ADD:
							Cell::alive_color = Color::GREEN;
							break;
						case EditMode::REMOVE:
							Cell::alive_color = Color::RED;
							break;
						case EditMode::TOGGLE:
							Cell::alive_color = Color::YELLOW;
							break;
						case EditMode::NONE:
							Cell::alive_color = Color::LIGHT_GREY;
							break;
					}
					Cell::border_color = Color::LIGHT_GREY;
				} else {
					Cell::alive_color = Cell::border_color = Color::GREY;
				}
				break;
			case SDL_MOUSEMOTION:
				if(!paused)
					break;
				{
					int x = event.motion.x / CELL_SIZE;
					int y = event.motion.y / CELL_SIZE;
					if(x >= grid.getCols() || y >= grid.getRows())
						break;
					switch(mode) {
						case EditMode::ADD:
							grid.cellAt(x, y).alive = true;
							break;
						case EditMode::REMOVE:
							grid.cellAt(x, y).alive = false;
							break;
						default:
							break;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if(!paused || mode != EditMode::TOGGLE)
					break;
				{
					int x = event.motion.x / CELL_SIZE;
					int y = event.motion.y / CELL_SIZE;
					if(x >= grid.getCols() || y >= grid.getRows())
						break;
					Cell& cell = grid.cellAt(x, y);
					cell.alive = !cell.alive;
				}
				break;
		}
	}
}

void GameOfLife::update() {
	if(!paused) {
		grid.update();
	}
}

void GameOfLife::gameloop() {
	while(running) {
		Uint32 currentFrameTime = SDL_GetTicks();
		if(currentFrameTime - drawFrameStart >= FRAME_DELAY) {
			draw();
			handleInput();
			// update();
			drawFrameStart = SDL_GetTicks();
		}
		currentFrameTime = SDL_GetTicks();
		if(currentFrameTime - updateFrameStart >= FRAME_DELAY * 4) {
			update();
			updateFrameStart = SDL_GetTicks();
		}
	}
}

GameOfLife::~GameOfLife() {
	for(TTF_Font* font: fonts) {
		TTF_CloseFont(font);
	}
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
