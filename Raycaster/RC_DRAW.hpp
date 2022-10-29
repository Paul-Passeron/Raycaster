/*
================================================================
RayCaster Engine
Paul Passeron 2022
RC_DRAW.hpp
================================================================
*/

#pragma once

#include "RC_GAMEOBJECT.hpp"
#include "RC_MATH.hpp"
#include <vector>

int get_Index(int i, int j, int width, bool is_RGBA) {
	/*
	================================================================
	get_index(int i, int j, int width) -> int:
	converts 2d array index (i, j) of size (width, height) to 1d
	array of size width * height.
	================================================================
	*/
	int index = i + j * width;
	if (is_RGBA) {
		return index * 4;
	}
	return index;
}


void drawScene(sf::RenderWindow& window, sf::Uint8*& pixels, sf::Texture& text, sf::Sprite& sprite) {
	/*
	================================================================
	drawScene(sf::RenderWindow &window, sf::Uint8* &pixels,
	sf::Texture &text, sf::Sprite &sprite) -> void:
	Draws the pixels array of size
	================================================================
	*/
	text.update(pixels); // setting the texture's pixels as ours.
	sprite.setTexture(text); // setting the sprite's texture as the texture containing our pixels.
	window.clear(); //clearing the previous image.
	window.draw(sprite); // drawing our pixels on the screen
	window.display(); // displaying the result.
}

void vPlotPixel(sf::Uint8*& pixels, float fColorCoeff, sf::Color& col, int i, int h, int WIDTH, int HEIGHT) {
	int index = get_Index(i, h, WIDTH, true); // index of the pixel to be written in the pixels array.
	//Checking if pixel is in bounds.
	if (index >= 0 && index < HEIGHT * WIDTH * 4 && col.a != 0) {
		pixels[index] = col.r * fColorCoeff; // Red component
		pixels[index + 1] = col.g * fColorCoeff; // Green component
		pixels[index + 2] = col.b * fColorCoeff; // Blue component
		//pixels[index + 3] = 255; // Alpha component
	}
}

void vDrawObjectLine(float fWallHeight, float fHeight, sf::Image& text_wall, float fColorCoeff, int x, int i, sf::Uint8*& pixels, int WIDTH) {
	/*
	================================================================
	drawObjectLine(float fWallHeight, float fHeight,
	sf::Image& text_wall, float fColorCoeff, int x, int i,
	sf::Uint8*& pixels, int WIDTH) -> void:
	Writes the right pixel slice of text_wall to the screen. It is
	currently used exclusevly to draw walls but will soon be used
	to draw objets, items, ennemies...
	================================================================
	*/
	int HEIGHT = (int)fHeight;
	for (int h = (int)(fHeight - fWallHeight) / 2; h < (int)(fWallHeight + fHeight) / 2; h++) {
		unsigned int y;// y is the y cordinate in texture space of the sampled pixel.
		//Calculating the sample value of the pixel (a float number between 0 and 1
		//indicating how high on the wall we are).
		float fWallHeightSample = fWallHeight / 2 - fHeight / 2 + h;
		//Calculating the y value associated with the sampled float.
		y = (fWallHeightSample / fWallHeight) * text_wall.getSize().y+1;
		//Making sure the pixel is within boundaries.
		if (y < 0) {
			y = 0;
		}
		if (y >= text_wall.getSize().y) {
			y = text_wall.getSize().y-1;
		}

		sf::Color col = text_wall.getPixel(x, y);
		vPlotPixel(pixels, fColorCoeff, col, i, h, WIDTH, HEIGHT);
	}
}

unsigned int iCalculateXSample(sf::Image text, geom::line lWall, geom::point inter) {
	unsigned int x;
	float fWallWidthSample = 1.0f / geom::inverse_distance(lWall.p2, inter);
	x = (fWallWidthSample * text.getSize().x)-1;
	// It isn't equal to 0 because fWallWidthSample was a float so the result of the last line isn't necesarily a multiple of text.getSize().x.
	return x % text.getSize().x;
}


geom::line lGetBillboarLine(rc::Item iItem, rc::Player pPlayer) {
	geom::point pP1 = geom::point(iItem.pGetPos().x - 0.5*iItem.fGetSize() * pPlayer.fGetSinAngle(), iItem.pGetPos().y + 0.5*iItem.fGetSize() * pPlayer.fGetCosAngle());
	geom::point pP2 = geom::point(iItem.pGetPos().x + 0.5*iItem.fGetSize() * pPlayer.fGetSinAngle(), iItem.pGetPos().y - 0.5*iItem.fGetSize() * pPlayer.fGetCosAngle());
	return geom::line(pP1, pP2);

}

int iFindBiggestInvDistIndex(std::vector<rc::Wall> wWalls, rc::Player pPlayer, geom::line lRay, float*& fWallInvDistances) {
	int iWallsNumber = wWalls.size();
	for (int iWallIndex = 0; iWallIndex < iWallsNumber; iWallIndex++) {
		rc::Wall wWall = wWalls[iWallIndex];
		geom::point pInter = geom::line_line_intersection(lRay, wWall.lGetWall(), geom::FALLS_WITHIN_SECOND_LINE_SEGMENT);
		float fInvDist = 0;
		if (pInter.exists) {
			fInvDist = geom::inverse_distance(pPlayer.pGetPos(), pInter);
		}
		fWallInvDistances[iWallIndex] = fInvDist;
	}
	return indexBiggestElem(fWallInvDistances, iWallsNumber);
}

int iFindBiggestInvDistIndex(std::vector<rc::Item> iItems, rc::Player pPlayer, geom::line lRay, float*&fItemInvDistances) {
	int iItemsNumber = iItems.size();
	for (int iItemIndex = 0; iItemIndex < iItemsNumber; iItemIndex++) {
		rc::Item iItem = iItems[iItemIndex];

		geom::line lBillboardLine = lGetBillboarLine(iItem, pPlayer);
		geom::point inter = geom::line_line_intersection(lRay, lBillboardLine, geom::FALLS_WITHIN_SECOND_LINE_SEGMENT);
		float fInvDist = 0;
		if (inter.exists) {
			fInvDist = geom::inverse_distance(pPlayer.pGetPos(), inter);
		}
		fItemInvDistances[iItemIndex] = fInvDist;
	}
	return indexBiggestElem(fItemInvDistances, iItemsNumber);
}

void vDrawLoop(rc::Scene sScene, rc::Player& pPlayer, std::vector<rc::Wall> wWalls, std::vector<rc::Item> iItems, sf::Uint8*& pixels) {
	float fWidth = (float)sScene.iGetWidth();
	float fHeight = (float)sScene.iGetHeight();
	float fRatio = fWidth / fHeight;
	float fMaxDist = 25;
	// 1D array with the inverse distances of the walls then the items.
	int iWallsNumber = wWalls.size();
	int iItemsNumber = iItems.size();
	float* fWallInvDistances = new float[iWallsNumber];
	float* fItemInvDistances = new float[iItemsNumber];
	for (int x = 0; x < sScene.iGetWidth(); x++) {
		float fRayAngle = pPlayer.fGetAngle() + pPlayer.fGetFov() * ((float)x / fWidth - 0.5f);
		float fCosRayAngle = cosf(fRayAngle);
		float fSinRayAngle = sinf(fRayAngle);
		geom::line lRay = geom::line(pPlayer.pGetPos(), geom::point(pPlayer.pGetPos().x + fCosRayAngle, pPlayer.pGetPos().y + fSinRayAngle));
		int iMaxWallIndex = iFindBiggestInvDistIndex(wWalls, pPlayer, lRay, fWallInvDistances);
		int iMaxItemIndex = iFindBiggestInvDistIndex(iItems, pPlayer, lRay, fItemInvDistances);
		rc::Wall wWall = wWalls[iMaxWallIndex];
		rc::Item iItem = iItems[iMaxItemIndex];

		geom::point pWallInter = geom::line_line_intersection(lRay, wWall.lGetWall(), geom::FALLS_WITHIN_SECOND_LINE_SEGMENT);
		float fWallDist = geom::distance_point_line(pPlayer.lGetCamera(), pWallInter);
		float fWallHeight = fRatio * fHeight / fWallDist;

		geom::line lBillboardLine = lGetBillboarLine(iItem, pPlayer);

		geom::point pItemInter = geom::line_line_intersection(lRay, lBillboardLine, geom::FALLS_WITHIN_SECOND_LINE_SEGMENT);
		//float fItemDist = geom::distance_point_line(pPlayer.lGetCamera(), pWallInter);
		float fItemDist = geom::inverse_distance(pPlayer.pGetPos(), pItemInter);
		float fItemRatio = iItem.fGetSize() * iItem.iGetTexture().getSize().y / iItem.iGetTexture().getSize().x;
		float fItemHeight = fRatio * fHeight * fItemDist;

		float fItemInvDistNaive = fItemInvDistances[iMaxItemIndex];
		float fWallInvDistNaive = fWallInvDistances[iMaxWallIndex];

		float fItemColorCoeff = fMaxDist / fItemDist;
		float fWallColorCoeff = fMaxDist / fWallDist;

		sf::Image iItemText = iItem.iGetTexture();
		sf::Image iWallText = wWall.iGetTexture();

		if (fItemColorCoeff > 1) { fItemColorCoeff = 1; }
		if (fWallColorCoeff > 1) { fWallColorCoeff = 1; }

		if (fItemInvDistNaive < fWallInvDistNaive) {
			if (pItemInter.exists) {
				vDrawObjectLine(fItemHeight, fHeight, iItemText, fItemColorCoeff, iCalculateXSample(iItemText, lBillboardLine, pItemInter), x, pixels, sScene.iGetWidth());
			}
			if (pWallInter.exists) {
				vDrawObjectLine(fWallHeight, fHeight, iWallText, fWallColorCoeff, iCalculateXSample(iWallText, wWall.lGetWall(), pWallInter), x, pixels, sScene.iGetWidth());
			}
		}
		else {
			if (pWallInter.exists) {
				vDrawObjectLine(fWallHeight, fHeight, iWallText, fWallColorCoeff, iCalculateXSample(iWallText, wWall.lGetWall(), pWallInter)/2, x, pixels, sScene.iGetWidth());
			}

			if (pItemInter.exists) {
				vDrawObjectLine(fItemHeight, fHeight, iItemText, fItemColorCoeff, iCalculateXSample(iItemText, lBillboardLine, pItemInter), x, pixels, sScene.iGetWidth());
			}
			
		}
	}
}