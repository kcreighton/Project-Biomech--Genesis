#include "Errors.h"

#include <cstdlib>

#include <iostream>
#include <SDL/SDL.h>

//Prints out an error message and exits the game
void fatalError(std::string errorString)
{
    std::cout << errorString << std::endl;
    std::cout << "Enter any key to quit...";
    int key;
    std::cin >> key;
    SDL_Quit();
	exit(34004);
}