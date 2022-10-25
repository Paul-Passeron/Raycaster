//RC_MAIN.cpp
/*
================================================================
RayCaster Engine
Paul Passeron 2022
================================================================
*/
#include <SDL.h>
#include <iostream>
#include <chrono>
#include "RC_MATH.h"
#include "RC_GEOM.h"
#include "RC_GAME.h"
/*
================================================================
Global variables
================================================================
*/

const char* TITLE = "Raycaster Engine"; //Window title
const int WIDTH = 600;
const int HEIGHT = 600;
const float PI = 3.141592653589793f;
geom::player pPlayer;
geom::point pWallp1;
geom::point pWallp2;
geom::line lWall;
float fov = PI / 3.0;



int main(int argc, char* args[]) {
	const Uint8* keyState;
	pWallp1.x = 0;
	pWallp1.y = 2;
	pWallp1.exists = true;
	pWallp2.x = 2;
	pWallp2.y = 2;
	pWallp2.exists = true;
	lWall.p1 = pWallp1;
	lWall.p2 = pWallp2;
	pPlayer.pos.x = 0;
	pPlayer.pos.y = 0;
	pPlayer.pos.exists = true;
	pPlayer.angle = PI/2;
	bool quit = false;
	SDL_Event event;
	SDL_Init(SDL_INIT_VIDEO);
	//Creating window
	SDL_Window* window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	//Creating renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	//Creating texture (=canvas)
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);
	//Creating an array of pixels (to draw on the screen)
	Uint32* pixels = new Uint32[WIDTH * HEIGHT];
	while (!quit) {
		std::cout << pPlayer.angle << std::endl;
		memset(pixels, 255, WIDTH * HEIGHT * sizeof(Uint32));
		gameloop(pPlayer, lWall, pixels, WIDTH, HEIGHT, fov);
		SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		SDL_PumpEvents();
		keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_ESCAPE])
			quit = true;
		if (keyState[SDL_SCANCODE_Q]) {
			pPlayer.angle -= 0.001;
		}
		if (keyState[SDL_SCANCODE_E]) {
			pPlayer.angle += 0.001;
		}
		if (keyState[SDL_SCANCODE_W]) {
			pPlayer.pos.x += cosf(pPlayer.angle) * 0.01;
			pPlayer.pos.y += sinf(pPlayer.angle) * 0.01;
		}
		if (keyState[SDL_SCANCODE_S]) {
			pPlayer.pos.x -= cosf(pPlayer.angle) * 0.01;
			pPlayer.pos.y -= sinf(pPlayer.angle) * 0.01;
		}
		if (keyState[SDL_SCANCODE_D]) {
			pPlayer.pos.x -= sinf(pPlayer.angle) * 0.01;
			pPlayer.pos.y += cosf(pPlayer.angle) * 0.01;
		}
		if (keyState[SDL_SCANCODE_A]) {
			pPlayer.pos.x += sinf(pPlayer.angle) * 0.01;
			pPlayer.pos.y -= cosf(pPlayer.angle) * 0.01;
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	delete[] pixels;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}