#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Runengine2D.h"

//a place where I am god
//to escape where I am not

namespace Runengine2D
{

	int init()
	{
		//Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		//Tell SDL that we want a double buffered window so we dont get
		//any flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}

}