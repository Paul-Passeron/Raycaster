#pragma once
//RC_GAME.h

#include "RC_GEOM.h"
#include <SDL.h>
#include "RC_DRAW.h"
#include <iostream>
float proj_screen_width = 10.0f;
void gameloop(geom::player player, geom::line wall, Uint32* pixels, int WIDTH, int HEIGHT, float fov) {
	float fWidth = (float)WIDTH;
	float fHeight = (float)HEIGHT;
	geom::line lCamera;
	geom::point p1;
	geom::point p2;
	p1.exists = true;
	p2.exists = true;
	p1.x = 0.5f * proj_screen_width * sin(player.angle) + player.pos.x;
	p1.y = -0.5f * proj_screen_width * cos(player.angle) + player.pos.y;
	p2.x = -0.5f * proj_screen_width * sin(player.angle) + player.pos.x;
	p2.y = 0.5f * proj_screen_width * cos(player.angle) + player.pos.y;
	lCamera.p1 = p1;
	lCamera.p2 = p2;


	for (int i = 0; i < WIDTH; i++) {
		float ray_angle = player.angle+fov/2*((float)i)/fWidth-fov/2;
		geom::point ray_origin;
		ray_origin.exists = true;
		geom::point ray_end;
		ray_end.exists = true;
		ray_origin.x = player.pos.x;
		ray_origin.y = player.pos.y;
		ray_end.x = ray_origin.x + cos(ray_angle);
		ray_end.y = ray_origin.y + sin(ray_angle);
		geom::line ray;
		ray.p1 = ray_origin;
		ray.p2 = ray_end;
		geom::point inter = line_line_intersection(ray, wall, geom::FALLS_WITHIN_SECOND_LINE_SEGMENT);
		float fWallHeight = 0;
		if (inter.exists) {
			float d_s = distance_squared(inter, player.pos);
			if (d_s > 0.01) {
				float d = geom::distance_point_line(lCamera, inter);
				fWallHeight = 1.26f * fHeight / d;
				if (fWallHeight > fHeight) {
					fWallHeight = fHeight;
				}
				for (int h = HEIGHT / 2 - (int)fWallHeight; h < (int)fWallHeight + HEIGHT / 2; h++) {

					int index = get_Index(i, h, WIDTH);
					if (index >= 0 && index < WIDTH * HEIGHT) {
						pixels[index] = 0;
					}
				}
			}
		}
		//std::cout << fWallHeight << std::endl;
	}

} 