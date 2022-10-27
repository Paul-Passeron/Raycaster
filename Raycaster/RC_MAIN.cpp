/*
#include <SDL.h>
#include <iostream>
#include <chrono>
#include "RC_MATH.hpp"
#include "RC_GEOM.hpp"
#include "RC_GAME.hpp"*/
/*
================================================================
Global variables
================================================================
*/
/*
const char* TITLE = "Raycaster Engine"; //Window title
const int WIDTH = 320;
const int HEIGHT = 240;
const float PI = 3.141592653589793f;
geom::player pPlayer;
geom::point pWallp1;
geom::point pWallp2;
geom::line lWall;
float fov = PI / 3.0;
const float fSpeed = 10.0f;
const float fOmega = PI/2;



int main(int argc, char* args[]) {
	const Uint8* keyState;
	pWallp1.x = 0;
	pWallp1.y = 5;
	pWallp1.exists = true;
	pWallp2.x = 2;
	pWallp2.y = 2;
	pWallp2.exists = true;
	lWall.p1 = pWallp1;
	lWall.p2 = pWallp2;
	pPlayer.pos.x = 0;
	pPlayer.pos.y = 0;
	pPlayer.pos.exists = true;
	pPlayer.angle = PI / 2;

	//==========================

	bool quit = false;
	SDL_Event event;
	SDL_Init(SDL_INIT_VIDEO);
	//Creating window
	SDL_Window* window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	//Creating renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	//Creating texture (=canvas)
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
	//Creating an array of pixels (to draw on the screen)
	SDL_Color* pixels = new SDL_Color[WIDTH * HEIGHT];
	std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;

	m_tp1 = std::chrono::system_clock::now();
	m_tp2 = std::chrono::system_clock::now();
	while (!quit) {
		m_tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
		m_tp1 = m_tp2;
		float fElapsedTime = elapsedTime.count();
		std::cout << 1 / fElapsedTime << "\n";
		//std::cout << pPlayer.angle << std::endl;
		memset(pixels, 0, WIDTH * HEIGHT * sizeof(Uint32));
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
			pPlayer.angle -= fOmega*fElapsedTime;
			float q = pPlayer.angle / PI;
			//pPlayer.angle -= ((float)((int)q)) * PI;
		}
		if (keyState[SDL_SCANCODE_E]) {
			pPlayer.angle += fOmega * fElapsedTime;
			float q = pPlayer.angle / (2*PI);
			//pPlayer.angle -= ((float)((int)q)) * 2 * PI;
		}
		if (keyState[SDL_SCANCODE_W]) {
			pPlayer.pos.x += cosf(pPlayer.angle) * fElapsedTime*fSpeed;
			pPlayer.pos.y += sinf(pPlayer.angle) * fElapsedTime*fSpeed;
		}
		if (keyState[SDL_SCANCODE_S]) {
			pPlayer.pos.x -= cosf(pPlayer.angle) * fElapsedTime*fSpeed;
			pPlayer.pos.y -= sinf(pPlayer.angle) * fElapsedTime*fSpeed;
		}
		if (keyState[SDL_SCANCODE_D]) {
			pPlayer.pos.x -= sinf(pPlayer.angle) * fElapsedTime*fSpeed;
			pPlayer.pos.y += cosf(pPlayer.angle) * fElapsedTime*fSpeed;
		}
		if (keyState[SDL_SCANCODE_A]) {
			pPlayer.pos.x += sinf(pPlayer.angle) * fElapsedTime*fSpeed;
			pPlayer.pos.y -= cosf(pPlayer.angle) * fElapsedTime*fSpeed;
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
*/




//RC_MAIN.cpp
/*
================================================================
RayCaster Engine
Paul Passeron 2022
================================================================
*/


#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

#include <iostream>
#include <chrono>

#include "RC_MATH.hpp"
#include "RC_GEOM.hpp"
#include "RC_GAME.hpp"

const char* TITLE = "Raycaster Engine"; //Window title
const int WIDTH = 1920/5;
const int HEIGHT = 1080/5;
const float PI = 3.141592653589793f;
geom::player pPlayer;
geom::point pWallp1;
geom::point pWallp2;
geom::line lWall;
float fov = PI / 3.0;
const float fSpeed = 10.0f;
const float fOmega = PI / 2;


sf::Image iScreenBuffer;
sf::Texture texture;
sf::Sprite sprite;
int main()
{
	float fElapsedTime;
	pWallp1.x = 0;
	pWallp1.y = 5;
	pWallp1.exists = true;
	pWallp2.x = 2;
	pWallp2.y = 2;
	pWallp2.exists = true;
	lWall.p1 = pWallp1;
	lWall.p2 = pWallp2;
	pPlayer.pos.x = 0;
	pPlayer.pos.y = 0;
	pPlayer.pos.exists = true;
	pPlayer.angle = PI / 2;
	sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), TITLE);
	//sf::RenderWindow window();
	/*sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);*/
	std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;

	m_tp1 = std::chrono::system_clock::now();
	m_tp2 = std::chrono::system_clock::now();
	texture.create(WIDTH, HEIGHT);
	while (window.isOpen())
	{
		m_tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
		m_tp1 = m_tp2;
		fElapsedTime = elapsedTime.count();

		std::cout << 1 / fElapsedTime << std::endl;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		memset(pixels, 0, WIDTH * HEIGHT * sizeof(sf::Color));

		gameloop(pPlayer, lWall, pixels, WIDTH, HEIGHT, fov);
		unsigned int w = WIDTH;
		unsigned int h = HEIGHT;
		texture.update(pixels);
		sprite.setTexture(texture);
		window.clear();
		window.draw(sprite);
		window.display();
		

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			pPlayer.angle -= fOmega * fElapsedTime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			pPlayer.angle += fOmega * fElapsedTime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			pPlayer.pos.x += cosf(pPlayer.angle) * fElapsedTime * fSpeed;
			pPlayer.pos.y += sinf(pPlayer.angle) * fElapsedTime * fSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			pPlayer.pos.x -= cosf(pPlayer.angle) * fElapsedTime * fSpeed;
			pPlayer.pos.y -= sinf(pPlayer.angle) * fElapsedTime * fSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			pPlayer.pos.x -= sinf(pPlayer.angle) * fElapsedTime * fSpeed;
			pPlayer.pos.y += cosf(pPlayer.angle) * fElapsedTime * fSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			pPlayer.pos.x += sinf(pPlayer.angle) * fElapsedTime * fSpeed;
			pPlayer.pos.y -= cosf(pPlayer.angle) * fElapsedTime * fSpeed;
		}
		
	}
return 0;
}