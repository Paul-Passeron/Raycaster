#pragma once
/*
================================================================
RayCaster Engine
Paul Passeron 2022
RC_GAMEOBJECT.hpp
================================================================
*/

#include "RC_GEOM.hpp"
#include <math.h>
#include <SFML/Graphics.hpp>
namespace rc {
	class GameObject {
	private:
		geom::point pPos;
		sf::Image iTexture;
	public:
		void vSetPos(geom::point pPos_) {
			pPos = pPos_;
		}
		void vSetTexture(sf::Image& iTexture_) {
			iTexture = iTexture_;
		}
		geom::point pGetPos() {
			return pPos;
		}
		sf::Image iGetTexture() {
			return iTexture;
		}
		GameObject(geom::point pPos_, sf::Image& iTexture_) {
			vSetPos(pPos_);
			vSetTexture(iTexture_);
		}
		GameObject(geom::point pPos_) {
			vSetPos(pPos_);
		}
		GameObject() {
			geom::point pPos_ = geom::point();
			GameObject(pPos_);
		}
	};

	class Wall : public GameObject {
	private:
		geom::line lWall;
	public:
		void vSetWall(geom::line lWall_) {
			lWall = lWall_;
		}
		geom::line lGetWall() {
			return lWall;
		}

		Wall(geom::point pPos_,sf::Image& iTexture_ , geom::line lL) : GameObject(pPos_, iTexture_) {
			vSetWall(lL);
		}
		Wall(geom::point pPos_, geom::line lL) : GameObject(pPos_) {
			vSetWall(lL);
		}
		Wall(geom::line lL) {
			vSetWall(lL);
		}
		Wall() {
			geom::line lL = geom::line();
			Wall(lL);
		}
	};

	class Entity : public GameObject {
	private:
		float fAngle;
		float fCosAngle;
		float fSinAngle;
		float fHealth;
	public:
		float fGetAngle() {
			return fAngle;
		}
		float fGetCosAngle() {
			return fCosAngle;
		}
		float fGetSinAngle() {
			return fSinAngle;
		}
		float fGetHealth() {
			return fHealth;
		}
		void vSetCosAngle(float x) {
			fCosAngle = x;
		}
		void vSetSinAngle(float x) {
			fSinAngle = x;
		}
		void vSetAngle(float x) {
			fAngle = x;
			vSetCosAngle(cosf(x));
			vSetSinAngle(sinf(x));
		}
		void vSetHealth(float x) {
			fHealth = x;
		}
		Entity(geom::point pP, sf::Image& iTexture_, float fH, float fA) :GameObject(pP, iTexture_) {
			vSetHealth(fH);
			vSetAngle(fA);
		}
		Entity() {
			vSetAngle(0);
		}
	};

	class Player : public Entity {
	private:
		float fFov;
		geom::line lCamera;
	public:
		void vSetFov(float x) {
			fFov = x;
		}
		void vSetCamera(geom::line lL) {
			lCamera = lL;
		}
		float fGetFov() {
			return fFov;
		}
		geom::line lGetCamera() {
			return lCamera;
		}
		Player(geom::point pP, sf::Image& iTexture_, float fAngle_, float fFov_, geom::line lL) : Entity(pP, iTexture_, 0, fAngle_) {
			vSetFov(fFov_);
			vSetCamera(lL);
		}
		Player() {
			vSetFov(0);
			geom::line lL = geom::line();
			vSetCamera(lL);
		}
	};
	class Enemy : public Entity {
	private:
		bool isDead;
	public:
		Enemy(geom::point pP, float a, sf::Image& iTexture_):Entity(pP, iTexture_, 0, a) {
			vSetTexture(iTexture_);
		}
		Enemy() {}
	};
	class Item : public GameObject {
	private:
		float size;
	public:
		void vSetSize(float x) {
			size = x;
		}
		float fGetSize() {
			return size;
		}
		Item(geom::point pP, sf::Image iTexture_, float x):GameObject(pP, iTexture_) {
			vSetSize(x);
		}
		Item(geom::point pP, sf::Image iTexture_):GameObject(pP, iTexture_) {
			vSetSize(1);
		}
		Item(geom::point pP, float x):GameObject(pP) {
			vSetSize(x);
		}
		Item(float x) {
			vSetSize(x);
		}
		Item(geom::point pP) : GameObject(pP) {}
		Item() {
			vSetSize(1);
		}
	};
	
	class Collectible : public Item {
	private:
		bool bIsCollected;
	public:
		void vSetIsCollected(bool bB) {
			bIsCollected = bB;
		}
		bool bGetIsCollected() {
			return bIsCollected;
		}
		Collectible(geom::point pP, sf::Image& iTexture_, bool bB = false) : Item(pP, iTexture_) {
			vSetIsCollected(bB);
		}
		Collectible(bool bB = false) {
			vSetIsCollected(bB);
		}
	};
	//Decoration is just an Item that isn't collectible -> Decoration is just Item.
}