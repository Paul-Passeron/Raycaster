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


void gameloop(geom::player player, geom::line wall, sf::Uint8* pixels, int WIDTH, int HEIGHT, float fov) {
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
	//Creating the camera line, which we maybe should'nt do here since it's always the same.
	//We should maybe pass it in as a parameter of the gameloop function.
	geom::point p1 = geom::point(0.5f * proj_screen_width * sin(player.angle) + player.pos.x, -0.5f * proj_screen_width * cos(player.angle) + player.pos.y);
	geom::point p2 = geom::point(-0.5f * proj_screen_width * sin(player.angle) + player.pos.x, 0.5f * proj_screen_width * cos(player.angle) + player.pos.y);
	geom::line lCamera = geom::line(p1, p2);
	//Looping over each row of pixel to determine the height of the wall to be displayed.
	for (int i = 0; i < WIDTH; i++) {
		//Calculating the angle of the ray we'll be casting.
		float ray_angle = player.angle+fov/2*((float)i)/fWidth-fov/2;
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
				fWallHeight = 1.26f * fHeight / d;
				//Capping the maximum wall heiht to be displayed on the screen.
				if (fWallHeight > fHeight) {
					fWallHeight = fHeight;
				}
				//Drawing the line of pixel at the correct size to draw a wall.
				for (int h = HEIGHT / 2 - (int)fWallHeight; h < (int)fWallHeight + HEIGHT / 2; h++) {
					int index = get_Index(i, h, WIDTH, true); // index of the pixel to be written in the pixels array.
					//Checking if pixel is in bounds.
					if (index >= 0 && index < WIDTH*HEIGHT*4) {
						sf::Uint8 comp = 255.0 * fWallHeight / fHeight;
						pixels[index] = comp; // Red component
						pixels[index+1] = 0; // Green component
						pixels[index+2] = 0; // Blue component
						pixels[index+3] = (sf::Uint8)255; // Alpha component
					}
				}
			}
		}
	}
} 

void handleKeyboardInput(float fElapsedTime, geom::player &pPlayer, float fOmega, float fSpeed) {
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
