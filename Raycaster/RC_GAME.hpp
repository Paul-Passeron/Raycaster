/*
================================================================
RayCaster Engine
Paul Passeron 2022
RC_GAME.hpp
================================================================
*/
#pragma once

#include "RC_GEOM.hpp"
#include "RC_DRAW.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
float proj_screen_width = 10.0f;


void gameloop(geom::player player, geom::line wall, sf::Uint8* pixels, int WIDTH, int HEIGHT, float fov, sf::Image& text_wall) {
	/*
	================================================================
	gameloop(geom::player player, geom::line wall,
	sf::Uint8* pixels, int WIDTH, int HEIGHT, float fov) -> void:
	Basically just draw the wall wall based on the fov, player pos,
	player angle and the dimensions of the screen. Nothing fancy
	yet, ther will be texture sampling, multiple walls (map) etc...
	================================================================
	*/
	//Casting WIDTH and HEIGHT to floats once and for all instead of doing it over and over again
	//in order to increase speed.
	const float fWidth = (float)WIDTH;
	const float fHeight = (float)HEIGHT;
	const float fRatio = fWidth / fHeight;
	const float PI = 3.14156;
	//Creating the camera line, which we maybe should'nt do here since it's always the same.
	//We should maybe pass it in as a parameter of the gameloop function.
	geom::point p1 = geom::point(0.5f * proj_screen_width * cos(player.angle+PI/2) + player.pos.x, 0.5f * proj_screen_width * sin(player.angle+PI/2) + player.pos.y);
	geom::point p2 = geom::point(0.5f * proj_screen_width * cos(player.angle-PI/2) + player.pos.x, 0.5f * proj_screen_width * sin(player.angle-PI/2) + player.pos.y);
	geom::line lCamera = geom::line(p1, p2);
	//Looping over each row of pixel to determine the height of the wall to be displayed.
	for (int i = 0; i < WIDTH; i++) {
		//Calculating the angle of the ray we'll be casting.
		float ray_angle = player.angle + fov / 2 * ((float)i) / fWidth - fov / 2;
		//Doesn't work as intended, we get a fisheye effect.
		//float ray_angle = atanf(2 * ((float)i) * proj_screen_width / fWidth * sinf(fov / 2.0f));

		//Creating the ray emitted from the player with angle ray_angle.
		geom::line ray = geom::line(player.pos, geom::point(player.pos.x + cos(ray_angle), player.pos.y + sin(ray_angle)));
		//Calculates the intrsection point between our ray and the wall.
		geom::point inter = line_line_intersection(ray, wall, geom::FALLS_WITHIN_SECOND_LINE_SEGMENT);
		float fWallHeight = 0;
		if (inter.exists) {
			float d_s = distance_squared(inter, player.pos);
			//To avoid division by 0.
			if (d_s > 0.001) {
				//we get the distance between the camera line and the point that our ray has hit on the wall.
				//It helps avoiding the fisheye effect we could get by calculating the distance to the player.
				float d = geom::distance_point_line(lCamera, inter);
				//Calculating the height of the wall to be displayed on screen (the actual one).
				//We could have another factor which would be the size of the acutal wall and we could use
				//that to render different walls at different heights. There is a few problems about that
				//that I still need to wrap my head around before doing this kind of stuff
				fWallHeight = fRatio * fHeight / d *2.0f;
				float fWallWidth = 1.0f / geom::inverse_distance(wall.p1, wall.p2);
				float fWallWidthSample = 1.0f / geom::inverse_distance(wall.p2, inter);
				unsigned int x; // x is the x cordinate in texture space of the sampled pixel.
				//sampling the pixel on the wall (we don't divide by the size of the wall because otherwise
				//the texture wold be stretched across the whole wall.
				x = (fWallWidthSample * text_wall.getSize().x);
				x = x % text_wall.getSize().x;
				//Drawing the line of pixel at the correct size to draw a wall.
				for (int h = (HEIGHT - (int)fWallHeight) / 2; h < ((int)fWallHeight + HEIGHT) / 2; h++) {
					unsigned int y;// y is the y cordinate in texture space of the sampled pixel.
					//Calculating the sample value of the pixel (a float number between 0 and 1
					//indicating how high on the wall we are).
					float fWallHeightSample = fWallHeight / 2 - fHeight / 2 + h;
					//Calculating the y value associated with the sampled float.
					y = (fWallHeightSample / fWallHeight) * text_wall.getSize().y;
					//Making sure the pixel is within boundaries.
					if (y < 0) {
						y = 0;
					}
					if (y >= text_wall.getSize().y) {
						y = text_wall.getSize().y - 1;
					}
					int index = get_Index(i, h, WIDTH, true); // index of the pixel to be written in the pixels array.
					//Checking if pixel is in bounds.
					if (index >= 0 && index < WIDTH * HEIGHT * 4) {
						sf::Color col = text_wall.getPixel(x, y);
						sf::Uint8 comp = 255.0 * fWallHeight / fHeight;
						pixels[index] = col.r; // Red component
						pixels[index + 1] = col.g; // Green component
						pixels[index + 2] = col.b; // Blue component
						pixels[index + 3] = (sf::Uint8)255; // Alpha component
					}
				}
			}

		}
	}
}

void handleKeyboardInput(float fElapsedTime, geom::player& pPlayer, float fOmega, float fSpeed) {
	/*
	================================================================
	handleKeyboardInput(float fElapsedTime, geom::player &pPlayer,
	float fOmega, float fSpeed) -> void:
	Makes the player move according to his inputs, for the moment it
	doesn't account for collisions or anything fancy.
	================================================================
	*/
	//Turning left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		pPlayer.angle -= fOmega * fElapsedTime;
	}
	//Turning right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		pPlayer.angle += fOmega * fElapsedTime;
	}
	//Moving forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		pPlayer.pos.x += cosf(pPlayer.angle) * fElapsedTime * fSpeed;
		pPlayer.pos.y += sinf(pPlayer.angle) * fElapsedTime * fSpeed;
	}
	//Moving backward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		pPlayer.pos.x -= cosf(pPlayer.angle) * fElapsedTime * fSpeed;
		pPlayer.pos.y -= sinf(pPlayer.angle) * fElapsedTime * fSpeed;
	}
	//Strafing right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		pPlayer.pos.x -= sinf(pPlayer.angle) * fElapsedTime * fSpeed;
		pPlayer.pos.y += cosf(pPlayer.angle) * fElapsedTime * fSpeed;
	}
	//Strafing left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		pPlayer.pos.x += sinf(pPlayer.angle) * fElapsedTime * fSpeed;
		pPlayer.pos.y -= cosf(pPlayer.angle) * fElapsedTime * fSpeed;
	}
	
}
