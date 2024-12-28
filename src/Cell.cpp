#include "Cell.hpp"
#include "Color.hpp"
#include <SDL2/SDL.h>

SDL_Color Cell::alive_color  = Color::GREY;
SDL_Color Cell::dead_color   = Color::WHITE;
SDL_Color Cell::border_color = Color::GREY;

extern int SDLHelper_SetRenderDrawColor(SDL_Renderer* renderer, const SDL_Color& color){
	return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Cell::draw(SDL_Renderer* renderer) {
	if(alive)
		SDLHelper_SetRenderDrawColor(renderer, alive_color);
	else
		SDLHelper_SetRenderDrawColor(renderer, dead_color);
	SDL_Rect rect = { .x = x * size, .y = y * size, .w = size, .h = size };
	SDL_RenderFillRect(renderer, &rect);
	SDLHelper_SetRenderDrawColor(renderer, border_color);
	SDL_RenderDrawRect(renderer, &rect);
}
