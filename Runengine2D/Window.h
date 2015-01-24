#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace Runengine2D
{

enum WindowFlags {INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4};
//bitwise

class Window
{
public:
	Window();
	~Window();

	void swapBuffer();

	int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

	int getScreenWidth() { _screenWidth; }
	int getScreenHeight() { _screenHeight; }

private:
	SDL_Window* _sdlWindow;
	int _screenWidth, _screenHeight;
};

}