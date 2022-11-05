#pragma once
/*
================================================================
RayCaster Engine
Paul Passeron 2022
RC_MAPLOADER.cpp
================================================================
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include "RC_GAMEOBJECT.hpp"

void vParseFile(sf::Image*& iTexts, std::string sFileToLoad, rc::Player& pPlayer, std::vector<rc::Wall> &wWalls, std::vector<rc::Item>& iItems, std::vector<rc::Collectible>& cCollectibles) {
	std::fstream fileStream;
	fileStream.open(sFileToLoad);
	bool bInWall = false;
	bool bInItem = false;
	bool bInPlayer = false;
	bool bInCollectible = false;
	while (fileStream) {
		std::string line;
		std::getline(fileStream, line, '\n');
		if (line[0] != '#' || line[1] != ':') {

			if (line == "#WBEGIN") {
				bInWall = true;
			}
			else if (line == "#WEND") {
				bInWall = false;
			}
			else if (line == "#IBEGIN") {
				bInItem = true;
			}
			else if (line == "#IEND") {
				bInItem = false;
			}
			else if (line == "#PBEGIN") {
				bInPlayer = true;
			}
			else if (line == "#PEND") {
				bInPlayer = false;
			}
			else if (line == "#CBEGIN") {
				std::cout << "Pol1" << std::endl;
				bInCollectible = true;
			}
			else if (line == "#CEND") {
				bInCollectible = false;
			}
			else if (bInWall) {
				float fX1, fX2, fX3, fX4, fs;
				int iTextIdentifier;
				std::stringstream ssin(line);
				if (ssin.good()) {
					ssin >> fX1;
				}
				if (ssin.good()) {
					ssin >> fX2;
				}
				if (ssin.good()) {
					ssin >> fX3;
				}
				if (ssin.good()) {
					ssin >> fX4;
				}
				if (ssin.good()) {
					ssin >> fs;
				}
				if (ssin.good()) {
					ssin >> iTextIdentifier;
				}
				geom::point pP1(fX1, fX2);
				geom::point pP2(fX3, fX4);
				geom::line lL(pP1, pP2);
				rc::Wall wWall(iTexts[iTextIdentifier], lL, fs);
				wWalls.push_back(wWall);
			}
			else if (bInItem) {
				float fX1, fX2, fX3;
				int iTextIdentifier;
				std::stringstream ssin(line);
				if (ssin.good()) {
					ssin >> fX1;
				}
				if (ssin.good()) {
					ssin >> fX2;
				}
				if (ssin.good()) {
					ssin >> fX3;
				}
				if (ssin.good()) {
					ssin >> iTextIdentifier;
				}

				geom::point pP1(fX1, fX2);
				rc::Item iItem(pP1, iTexts[iTextIdentifier], fX3);
				iItems.push_back(iItem);
			}
			else if (bInPlayer) {
				float fX1, fX2;
				std::stringstream ssin(line);
				if (ssin.good()) {
					ssin >> fX1;
				}
				if (ssin.good()) {
					ssin >> fX2;
				}
				geom::point pP(fX1, fX2);
				pPlayer.vSetPos(pP);
			}
			else if (bInCollectible) {
				float fX1, fX2;
				int iTextIdentifier;
				std::stringstream ssin(line);
				if (ssin.good()) {
					ssin >> fX1;
				}
				if (ssin.good()) {
					ssin >> fX2;
				}
				if (ssin.good()) {
					ssin >> iTextIdentifier;
				}
				geom::point pP1(fX1, fX2);
				rc::Collectible cCollectible(pP1, iTexts[iTextIdentifier], false);
				cCollectibles.push_back(cCollectible);
			}
		}
	}
	fileStream.close();

}