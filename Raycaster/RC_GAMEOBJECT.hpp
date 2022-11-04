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
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>


namespace rc {

	typedef enum GAMEOBJECT_TYPE {
		WALL, //0
		ITEM, //1
	};
	class GameObject {
	private:
		geom::point pPos = geom::point();
		sf::Image iTexture = sf::Image();
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
		GameObject() {}

	};

	class Renderable : public GameObject {
	private:
		int startHeight;
		int Height;
		int x;

	public:
		GAMEOBJECT_TYPE t = ITEM;
		Renderable() {
			startHeight = 0;
			Height = 0;
			x = 0;
		}
		Renderable(geom::point pPos_, sf::Image& iTexture_, int sh, int h, GAMEOBJECT_TYPE type, int fx) :GameObject(pPos_, iTexture_) {
			startHeight = sh;
			Height = h;
			t = type;
			x = fx;
		}

		float fGetHeight() {
			return Height;
		}
		float fGetStartHeight() {
			return startHeight;
		}
		int iGetX() {
			return x;
		}
	};

	class Wall : public GameObject {
	private:
		geom::line lWall;
		float fSize = 1;
	public:
		void vSetWall(geom::line lWall_) {
			lWall = lWall_;
		}
		geom::line lGetWall() {
			return lWall;
		}

		Wall(geom::point pPos_, sf::Image& iTexture_, geom::line lL) : GameObject(pPos_, iTexture_) {
			vSetWall(lL);
		}
		Wall(geom::point pPos_, geom::line lL) : GameObject(pPos_) {
			vSetWall(lL);
		}
		Wall(sf::Image& iTexture_, geom::line lL) : GameObject(geom::point(), iTexture_) {
			vSetWall(lL);

		}
		Wall(geom::line lL) {
			vSetWall(lL);
		}
		Wall() {
			vSetWall(geom::line());
		}
		Wall(geom::point pPos_, sf::Image& iTexture_, geom::line lL, float s) : GameObject(pPos_, iTexture_) {
			vSetWall(lL);
			fSize = s;
		}
		Wall(geom::point pPos_, geom::line lL, float s) : GameObject(pPos_) {
			vSetWall(lL);
			fSize = s;

		}
		Wall(sf::Image& iTexture_, geom::line lL, float s) : GameObject(geom::point(), iTexture_) {
			vSetWall(lL);
			fSize = s;


		}
		Wall(geom::line lL, float s) {
			vSetWall(lL);
			fSize = s;

		}
		Wall(float s) {
			vSetWall(geom::line());
			fSize = s;

		}
		float fGetSize() {
			return fSize;
		}

	};
	//Need to add Interactable wall class.
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
		bool isInit = true;
	public:
		int nAmmo = 0;
		bool bCanShoot;
		bool bIsTimerActive;
		float fPlayerTime;
		float fMaxPlayerTime;
		
		void vOnTimerEnd() {
			bCanShoot = true;
		}

		void vOnTimerStart() {
			bCanShoot = false;
		}
		void vSetTimer(float max) {
			bIsTimerActive = true;
			fPlayerTime = 0;
			fMaxPlayerTime = max;
			vOnTimerStart();
		}
		void vUpdateTimer(float fElapsedTime) {
			if (bIsTimerActive) {
				fPlayerTime += fElapsedTime;
			}
			if (fPlayerTime > fMaxPlayerTime) {
				bIsTimerActive = false;
				vOnTimerEnd();
			}
		}
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
		Player(geom::point pP, sf::Image iTexture_, float fAngle_, float fFov_, geom::line lL) : Entity(pP, iTexture_, 0, fAngle_) {
			vSetFov(fFov_);
			vSetCamera(lL);
			nAmmo = 0;
			bIsTimerActive = false;
			bCanShoot = true;

		}
		Player() {
			vSetFov(0);
			geom::line lL = geom::line();
			vSetCamera(lL);
			isInit = false;
			nAmmo = 0;
			bIsTimerActive = false;
			bCanShoot = true;

		}
		bool bGetIsInit() {
			return isInit;
		}
	};
	class Enemy : public Entity {
	private:
		bool isDead;
	public:
		Enemy() {
			isDead = false;
		}
		Enemy(geom::point pP, float a, sf::Image& iTexture_) :Entity(pP, iTexture_, 0, a) {
			vSetTexture(iTexture_);
			isDead = false;
		}

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
		Item(geom::point pP, sf::Image iTexture_, float x) :GameObject(pP, iTexture_) {
			vSetSize(x);
		}
		Item(geom::point pP, sf::Image iTexture_) :GameObject(pP, iTexture_) {
			vSetSize(1);
		}
		Item(geom::point pP, float x) :GameObject(pP) {
			vSetSize(x);
		}
		Item(float x) {
			vSetSize(x);
		}
		Item() {
			vSetSize(1);
		}
		Item(geom::point pP) : GameObject(pP) {
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
		Collectible() {
			vSetIsCollected(false);
		}
		void vUpdate(Player& pPlayer) {
			//Function to be overriden by the user.
			if (geom::distance_squared(pPlayer.pGetPos(), pGetPos()) < 1) {
				bIsCollected = true;
				std::cout << "Ammo Collected!";
				pPlayer.nAmmo += 10;
				std::cout << pPlayer.nAmmo << std::endl;
			}

		}
	};
	//Decoration is just an Item that isn't collectible -> Decoration is just Item.

	class Scene {
	private:
		int iWidth;
		int iHeight;
		const char* sTitle;
		sf::RenderWindow window;

	public:
		void vSetWidth(int x) {
			iWidth = x;
		}
		void vSetHeight(int x) {
			iHeight = x;
		}
		void vSetTitle(const char* s) {
			sTitle = s;
		}
		int iGetWidth() {
			return iWidth;
		}
		int iGetHeight() {
			return iHeight;
		}
		const char* sGetTitle() {
			return sTitle;
		}
		Scene(int w, int h, const char* s) {
			vSetHeight(h);
			vSetWidth(w);
			vSetTitle(s);
		}
		Scene() {
			vSetHeight(600);
			vSetWidth(600);
			vSetTitle("");
		}

		void vInit() {
			window.create(sf::VideoMode(iGetWidth(), iGetHeight()), sGetTitle());
		}
		void vSetIcon(std::string filePath) {
			auto icon = sf::Image{};
			icon.loadFromFile(filePath);
			window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
		}
		bool bIsOpen() {
			return window.isOpen();
		}
	};
}