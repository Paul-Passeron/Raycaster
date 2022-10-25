//RC_MAIN.cpp
/*
================================================================
RayCaster Engine
Paul Passeron 2022
================================================================
*/
#include <SDL.h>
#include <iostream>
#include <chrono>
#include "RC_MATH.h"


/*
================================================================
Global variables
================================================================
*/



bool init() {
	return true;
}

void close() {}

void gameloop();



int main(int argc, char* args[]) {
	// Main loop flag
	if (!init()) {
		return 0;
	}
	bool quit = false;
	while (!quit) {
		//Do stuff
		quit = true;
	}
	std::cout << fast_inverse_square_root(2.3f) << std::endl;
	close();
	return 0;
}