#include "game.h"


float deltaTime = 0.0f;
float accumulator = 0.0f;
float currentTime;
float globalTime = 0.0f;


float hireTimeInSeconds()
{
	float t = SDL_GetTicks();
	t *= 0.001f;

	return t;
}

int init_sdl()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	return 0;
}

void game_init(Game * game)
{
	game->sdl_window = SDL_CreateWindow(
		"Mandelbrot Set Renderer",
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, 
		SCREEN_WIDTH, 
		SCREEN_HEIGHT, 
		SDL_WINDOW_SHOWN
		);
	game->sdl_renderer = SDL_CreateRenderer(
		game->sdl_window,
		-1,
		SDL_RENDERER_ACCELERATED
		);

	currentTime = hireTimeInSeconds();

	SDL_SetRenderDrawColor(game->sdl_renderer, 50, 50, 50, 255);

}

void game_run(Game * game)
{
	SDL_Event e;

	while (!game->quit)
	{

		int startTicks = SDL_GetTicks();
		float newTime = hireTimeInSeconds();
		float frameTime = newTime - currentTime;

		deltaTime = frameTime;

		currentTime = newTime;
		accumulator += frameTime;

		while (accumulator >= timeStep)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					game->quit = true;
					return;
				}
			}

			accumulator -= timeStep;
		}

		SDL_SetRenderDrawColor(game->sdl_renderer, 50, 50, 50, 255);

		game_render_window(game);

		int frameTicks = SDL_GetTicks() - startTicks;
		if (frameTicks < 1000/REFRESH_RATE)
		{
			SDL_Delay(1000/REFRESH_RATE - frameTicks);
		}
	}
}

void game_render_window(Game * game)
{

}

void game_cleanup(Game * game)
{
	SDL_DestroyRenderer(game->sdl_renderer);
	SDL_DestroyWindow(game->sdl_window);

	SDL_Quit();
}
