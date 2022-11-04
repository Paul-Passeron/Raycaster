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
#include "RC_GAMEOBJECT.hpp"
float proj_screen_width = 10.0f;


void vShoot(rc::Player& pPlayer) {
	if (!pPlayer.bIsTimerActive) {
		if (!pPlayer.bCanShoot) {
			pPlayer.vSetTimer(0.33);
		}
		else {
			if (pPlayer.nAmmo > 0) {
				pPlayer.bCanShoot = false;
				pPlayer.nAmmo--;
				std::cout << "Pow! ";
				std::cout << pPlayer.nAmmo << std::endl;
			}
			else {
				std::cout << "Not enough ammo!" << std::endl;
			}
		}
	}
}


void handleKeyboardInput(float fElapsedTime, rc::Player& pPlayer, float fOmega, float fSpeed, int frameCounter) {
	if (pPlayer.bIsTimerActive) {
		pPlayer.vUpdateTimer(fElapsedTime);
	}
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
		pPlayer.vSetAngle(pPlayer.fGetAngle() - fOmega * fElapsedTime);
	}
	//Turning right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		pPlayer.vSetAngle(pPlayer.fGetAngle() + fOmega * fElapsedTime);
	}
	//Moving forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		geom::point pP;
		pP.x = pPlayer.pGetPos().x + pPlayer.fGetCosAngle() * fElapsedTime * fSpeed;
		pP.y = pPlayer.pGetPos().y + pPlayer.fGetSinAngle() * fElapsedTime * fSpeed;
		pPlayer.vSetPos(pP);
	}
	//Moving backward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		geom::point pP;
		pP.x = pPlayer.pGetPos().x - cosf(pPlayer.fGetAngle()) * fElapsedTime * fSpeed;
		pP.y = pPlayer.pGetPos().y - sinf(pPlayer.fGetAngle()) * fElapsedTime * fSpeed;
		pPlayer.vSetPos(pP);
	}
	//Strafing right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		geom::point pP;
		pP.x = pPlayer.pGetPos().x - sinf(pPlayer.fGetAngle()) * fElapsedTime * fSpeed;
		pP.y = pPlayer.pGetPos().y + cosf(pPlayer.fGetAngle()) * fElapsedTime * fSpeed;
		pPlayer.vSetPos(pP);
	}
	//Strafing left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		geom::point pP;
		pP.x = pPlayer.pGetPos().x + sinf(pPlayer.fGetAngle()) * fElapsedTime * fSpeed;
		pP.y = pPlayer.pGetPos().y - cosf(pPlayer.fGetAngle()) * fElapsedTime * fSpeed;
		pPlayer.vSetPos(pP);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		vShoot(pPlayer);
	}
}
