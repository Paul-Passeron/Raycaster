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
#include <algorithm>

rc::Player pPlayerloc;
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
	//window.display(); // displaying the result.
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

void vDrawObjectLine(float fWallHeight, float fHeight, sf::Image& text_wall, float fColorCoeff, int x, int i, sf::Uint8*& pixels, int WIDTH, int start) {
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
	for (int h = start + fWallHeight; h > start; h--) {
		unsigned int y;// y is the y cordinate in texture space of the sampled pixel.
		//Calculating the sample value of the pixel (a float number between 0 and 1
		//indicating how high on the wall we are).
		float fWallHeightSample = h - start;
		//Calculating the y value associated with the sampled float.
		y = (1 - fWallHeightSample / fWallHeight) * text_wall.getSize().y + 1;
		//Making sure the pixel is within boundaries.
		if (y < 0) {
			y = 0;
		}
		if (y >= text_wall.getSize().y) {
			y = text_wall.getSize().y - 1;
		}

		sf::Color col = text_wall.getPixel(x, y);
		vPlotPixel(pixels, fColorCoeff, col, i, HEIGHT - h, WIDTH, HEIGHT);
	}
}

unsigned int iCalculateXSample(sf::Image text, geom::line lWall, geom::point inter) {
	unsigned int x;
	float fWallWidthSample = 1.0f / geom::inverse_distance(lWall.p2, inter);
	x = (fWallWidthSample * text.getSize().x) - 1;
	// It isn't equal to 0 because fWallWidthSample was a float so the result of the last line isn't necesarily a multiple of text.getSize().x.
	if (text.getSize().x != 0)
	{
		return x % text.getSize().x;
	}
}


geom::line lGetBillboardLine(rc::Item iItem, rc::Player pPlayer) {
	geom::point pP1 = geom::point(iItem.pGetPos().x - 0.5 * iItem.fGetSize() * pPlayer.fGetSinAngle(), iItem.pGetPos().y + 0.5 * iItem.fGetSize() * pPlayer.fGetCosAngle());
	geom::point pP2 = geom::point(iItem.pGetPos().x + 0.5 * iItem.fGetSize() * pPlayer.fGetSinAngle(), iItem.pGetPos().y - 0.5 * iItem.fGetSize() * pPlayer.fGetCosAngle());
	return geom::line(pP2, pP1);

}

geom::line lGetBillboardLine(rc::Collectible iItem, rc::Player pPlayer) {
	geom::point pP1 = geom::point(iItem.pGetPos().x - 0.5 * iItem.fGetSize() * pPlayer.fGetSinAngle(), iItem.pGetPos().y + 0.5 * iItem.fGetSize() * pPlayer.fGetCosAngle());
	geom::point pP2 = geom::point(iItem.pGetPos().x + 0.5 * iItem.fGetSize() * pPlayer.fGetSinAngle(), iItem.pGetPos().y - 0.5 * iItem.fGetSize() * pPlayer.fGetCosAngle());
	return geom::line(pP2, pP1);

}




bool operator >= (rc::Renderable& rR1, rc::Renderable& rR2) {
	return rR1.fGetStartHeight() >= rR2.fGetStartHeight();
}

void sort(std::vector<rc::Renderable>& rToRender) {
	for (int i = 0; i < rToRender.size(); i++) {
		for (int j = 0; j <= i; j++) {
			if (rToRender[i] >= rToRender[j]) {
				std::swap(rToRender[i], rToRender[j]);
			}
		}
	}
}

void vDrawLoop(rc::Scene& sScene, rc::Player& pPlayer, std::vector<rc::Wall> wWalls, std::vector<rc::Item> iItems, std::vector<rc::Collectible> cCollectibles, sf::Uint8*& pixels) {

	float fWidth = (float)sScene.iGetWidth();
	float fHeight = (float)sScene.iGetHeight();
	float fRatio = fWidth / fHeight;
	float fMaxDist = 10;

	int iWallsNumber = wWalls.size();
	int iItemsNumber = iItems.size();
	int iCollectibleNumber = cCollectibles.size();

	for (int x = 0; x < sScene.iGetWidth(); x++) {

		float fRayAngle = pPlayer.fGetAngle() + pPlayer.fGetFov() * ((float)x / fWidth - 0.5f);
		float fCosRayAngle = cosf(fRayAngle);
		float fSinRayAngle = sinf(fRayAngle);
		geom::line lRay = geom::line(pPlayer.pGetPos(), geom::point(pPlayer.pGetPos().x + fCosRayAngle, pPlayer.pGetPos().y + fSinRayAngle));
		std::vector<rc::Renderable> rToRender;
		pPlayerloc.vSetPos(pPlayer.pGetPos());
		//WALLS
		for (int iWallIndex = 0; iWallIndex < iWallsNumber; iWallIndex++) {
			rc::Wall wWall = wWalls[iWallIndex];
			geom::point pWallInter = geom::line_line_intersection(lRay, wWall.lGetWall(), geom::FALLS_WITHIN_SECOND_LINE_SEGMENT);
			float fWallDist = geom::distance_point_line(pPlayer.lGetCamera(), pWallInter);
			if (pWallInter.exists) {
				//std::cout << iWallIndex << std::endl;
				float fWallDist = geom::distance_point_line(pPlayer.lGetCamera(), pWallInter);
				float fWallHeight = fRatio * fHeight / fWallDist;
				int h = fWallHeight * 0.9;
				int sh = (sScene.iGetHeight() - fWallHeight) / 2;

				sf::Image text = wWall.iGetTexture();
				float xsample = iCalculateXSample(text, wWall.lGetWall(), pWallInter);
				rc::Renderable rR = rc::Renderable(wWall.pGetPos(), text, sh, h, rc::WALL, xsample);
				rToRender.push_back(rR);
			}
		}
		//ITEMS
		for (int iItemIndex = 0; iItemIndex < iItemsNumber; iItemIndex++) {
			rc::Item iItem = iItems[iItemIndex];
			geom::line lBillboardLine = lGetBillboardLine(iItem, pPlayer);
			geom::point pItemInter = geom::line_line_intersection(lRay, lBillboardLine, geom::FALLS_WITHIN_SECOND_LINE_SEGMENT);

			if (pItemInter.exists) {
				float fItemDist = geom::inverse_distance(pPlayer.pGetPos(), pItemInter);
				float fItemRatio = iItem.fGetSize() * iItem.iGetTexture().getSize().y / iItem.iGetTexture().getSize().x;
				int temp = fRatio * fHeight * fItemDist;
				int sh = (sScene.iGetHeight() - temp * fItemRatio) / 2;
				int h = temp * fItemRatio;
				sf::Image text = iItem.iGetTexture();
				float xsample = iCalculateXSample(text, lBillboardLine, pItemInter);
				rc::Renderable rR = rc::Renderable(pItemInter, text, sh, h, rc::ITEM, xsample);
				rToRender.push_back(rR);
			}
		}
		//Collectibles
		for (int iCollectibleIndex = 0; iCollectibleIndex < iCollectibleNumber; iCollectibleIndex++) {
			rc::Collectible cCollectible = cCollectibles[iCollectibleIndex];
			if (!cCollectible.bGetIsCollected()) {
				geom::line lBillboardLine = lGetBillboardLine(cCollectible, pPlayer);
				geom::point pCollectibleInter = geom::line_line_intersection(lRay, lBillboardLine, geom::FALLS_WITHIN_SECOND_LINE_SEGMENT);
				if (pCollectibleInter.exists) {
					float fItemDist = geom::inverse_distance(pPlayer.pGetPos(), pCollectibleInter);
					float fItemRatio = cCollectible.iGetTexture().getSize().y / cCollectible.iGetTexture().getSize().x;
					int temp = fRatio * fHeight * fItemDist;
					int sh = (sScene.iGetHeight() - temp * fItemRatio) / 2;
					int h = temp * fItemRatio;
					sf::Image text = cCollectible.iGetTexture();
					float xsample = iCalculateXSample(text, lBillboardLine, pCollectibleInter);
					rc::Renderable rR = rc::Renderable(pCollectibleInter, text, sh, h, rc::WALL, xsample);
					rToRender.push_back(rR);
				}
			}
		}
		//Sorting rRToRender
		const int size = rToRender.size();
		std::vector<float> fToSort;
		std::vector<int> index;
		for (int i = 0; i < size; i++) {
			index.push_back(i);
			float d = geom::distance_squared(rToRender[i].pGetPos(), pPlayer.pGetPos());
			fToSort.push_back(d);
		}
		sort(rToRender);
		for (int i = 0; i < rToRender.size(); i++) {
			rc::Renderable rR = rToRender[i];
			sf::Image text = rR.iGetTexture();
			unsigned int xsample = rR.iGetX();
			float fcolorcoeff = rR.fGetHeight() / fHeight * 4.0f;
			if (fcolorcoeff > 1) {
				fcolorcoeff = 1.0f;
			}
			vDrawObjectLine(rR.fGetHeight(), fHeight, text, fcolorcoeff, xsample, x, pixels, (int)fWidth, rR.fGetStartHeight());
		}
	}


}

