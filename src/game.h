#pragma once
#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "globals.h"


typedef struct Game
{
	SDL_Window * sdl_window;

	SDL_Renderer * sdl_renderer;

	bool quit;

} Game;


float hireTimeInSeconds();

const float timeStep = 0.01f;

extern float deltaTime;

extern float accumulator;

extern float currentTime;

extern float globalTime;


int init_sdl();

void game_init(Game * game);

void game_run(Game * game);

void game_render_window(Game * game);

void game_cleanup(Game * game);