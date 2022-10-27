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

#include "RC_MATH.hpp"
#include "RC_GEOM.hpp"
#include "RC_GAME.hpp"

const char* TITLE = "Raycaster Engine"; //Window title
const int WIDTH = 1920/5; //Window width
const int HEIGHT = 1080/5; //Window height
const float PI = 3.141592653589793f; //The constant pi, used extensively in calculations.
geom::player pPlayer; // The player variable, used during the whole gameloop.
float fov = PI / 3.0; // The player's field of view, could become one of the player's struct field.
const float fSpeed = 10.0f; // The speed at which the player moves (nothing fancy, no sliding / friction).
const float fOmega = PI / 2; // The speed at which the player can turn.


sf::Texture texture; // Texture that will be displayed on the screen.
sf::Sprite sprite; // Sprite that will be displayed on the screen.
int main()
{
	
	//Declaring a test wall, will later be loaded from a file.
	geom::line lWall = geom::line(geom::point(0, 5), geom::point(2, 2));
	//setting the player starting angle.
	pPlayer.angle = PI / 2;
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
	
	//Loop that stays open while the winodw is open, we put the gameplay code in here.
	while (window.isOpen())
	{
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

		//Reset pixels to all black.
		memset(pixels, 0, WIDTH * HEIGHT * sizeof(sf::Color));
		//GameLoop ie drawig every walls etc on pixels buffer
		gameloop(pPlayer, lWall, pixels, WIDTH, HEIGHT, fov);
		//Draws the pixels buffer on the screen.
		drawScene(window, pixels, texture, sprite);
		// Handling input (player movement)
		handleKeyboardInput(fElapsedTime, pPlayer, fOmega, fSpeed);
	}
return 0;
}