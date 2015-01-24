#include <iostream>
#include "Game.h"
#include <GLM/glm.hpp>

//note: GLEW sucks because all its commands are #defines and the defines make it hard to tell what objects are being modified.
//I am still fuzzy on how to poop

int main(int argc, char** argv) {

	//Uint8 transferState getTransferState()
	//I think it would be cool to exit out of the game at one point for dramatic effect
	//and then load where it left on the next start up

	//menu function
	

    Game MainGame;
    MainGame.run();

    return 0;
}