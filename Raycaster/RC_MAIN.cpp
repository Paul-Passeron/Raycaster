/*
================================================================
RayCaster Engine
Paul Passeron 2022
RC_MAIN.cpp
================================================================
*/


#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

#include <iostream>
#include <chrono>
#include <algorithm>

#include "RC_MATH.hpp"
#include "RC_GEOM.hpp"
#include "RC_GAME.hpp"
#include "RC_GAMEOBJECT.hpp"
#include "RC_MAPLOADER.hpp"

const char* TITLE = "Raycaster Engine"; //Window title
const int WIDTH = 320; //Window width
const int HEIGHT = 200; //Window height
const float PI = 3.141592653589793f; //The constant pi, used extensively in calculations.
float fov = 72.0f*PI/180.0f; // The player's field of view, could become one of the player's struct field.
const float fSpeed = 15.0f; // The speed at which the player moves (nothing fancy, no sliding / friction).
const float fOmega = PI / 2; // The speed at which the player can turn.


sf::Texture texture; // Texture that will be displayed on the screen.
sf::Sprite sprite; // Sprite that will be displayed on the screen.


int main()
{

	sf::Image iWallText;
	iWallText.loadFromFile("texture.png");
	sf::Image iItemText;
	iItemText.loadFromFile("item0.png");
	sf::Image* Textures = new sf::Image[2];
	Textures[0] = iWallText;
	Textures[1] = iItemText;
	std::vector<rc::Wall> wWalls;
	std::vector<rc::Item> iItems;
	rc::Player pPlayer = rc::Player(geom::point(0, 0), sf::Image(), 0.0f, fov, geom::line());
	vParseFile(Textures, "RC_MAP.rcmap", pPlayer, wWalls, iItems);

	delete[] Textures;
	//Creating the buffer that contains every pixel that will
	//be rendered to the screen (RGBA).
	sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];
	//Creating the window
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), TITLE);
	//Creating the texture used to display on the screen.
	texture.create(WIDTH, HEIGHT);
	//Creating time variables used to calculate the elapsed time
	//between two frames.
	std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;
	m_tp1 = std::chrono::system_clock::now();
	m_tp2 = std::chrono::system_clock::now();
	float fElapsedTime;
	
	//Loading and setting the window's icon.
	auto icon = sf::Image{};
	icon.loadFromFile("icone.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	
	rc::Scene sScene(WIDTH, HEIGHT, "RC");
	//Loop that stays open while the winodw is open, we put the gameplay code in here.
	while (window.isOpen())
	{
		
		geom::point pP1 = geom::point(-0.5f * proj_screen_width * pPlayer.fGetSinAngle() + pPlayer.pGetPos().x, 0.5f * proj_screen_width * pPlayer.fGetCosAngle() + pPlayer.pGetPos().y);
		geom::point pP2 = geom::point(0.5f * proj_screen_width * pPlayer.fGetSinAngle() + pPlayer.pGetPos().x, -0.5f * proj_screen_width * pPlayer.fGetCosAngle() + pPlayer.pGetPos().y);
		geom::line lCamera = geom::line(pP1, pP2);
		pPlayer.vSetCamera(lCamera);
		//Calcultating the elapsed time between the previous frame and this one.
		m_tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
		m_tp1 = m_tp2;

		fElapsedTime = elapsedTime.count();
		//std::cout << 1 / fElapsedTime << std::endl; //Prints the fps count to the console.
		//Creating an event to handle wether or not we have closed the window.
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}

		//Reset pixels to all black.
		//memset(pixels,sf::Color(255, 0, 0).toInteger(), WIDTH * HEIGHT);
		for (int i = 0; i < WIDTH; i++) {
			for (int j = 0; j < HEIGHT; j++) {
				int index = get_Index(i, j, WIDTH, true);
				sf::Color col;
				if (j < HEIGHT / 2) {
					col = sf::Color(50, 50, 50);
				}
				else {
					col = sf::Color(100, 100, 100);
				}
				pixels[index] = col.r;
				pixels[index + 1] = col.g;
				pixels[index + 2] = col.b;
				pixels[index + 3] = 255;
			}
		}
		//GameLoop ie drawig every walls etc on pixels buffer
		//gameloop(pPlayer, lWall, pixels, WIDTH, HEIGHT, fov);
		//gameloop(pPlayer, lWallsArray, pixels, WIDTH, HEIGHT, fov, text_wall);
		
		vDrawLoop(sScene, pPlayer, wWalls, iItems, pixels);
		//Draws the pixels buffer on the screen.
		drawScene(window, pixels, texture, sprite);
		// Handling input (player movement)
		//pPlayer.angle -= fov / 4; //It corrects some problems with plyer movement.
		handleKeyboardInput(fElapsedTime, pPlayer, fOmega, fSpeed);
		//pPlayer.angle += fov / 4;

	}
return 0;
}