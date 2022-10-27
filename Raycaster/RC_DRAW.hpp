/*
================================================================
RayCaster Engine
Paul Passeron 2022
RC_DRAW.hpp
================================================================
*/

#pragma once

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


void drawScene(sf::RenderWindow &window, sf::Uint8* &pixels, sf::Texture &text, sf::Sprite &sprite) {
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