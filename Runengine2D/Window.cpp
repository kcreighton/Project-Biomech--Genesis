#include "Window.h"
#include "Errors.h"

namespace Runengine2D
{

	Window::Window() :
		_screenWidth(),
		_screenHeight()
	{
	}

	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{

		Uint32 flags = SDL_WINDOW_OPENGL;
		if (currentFlags & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		//Open an SDL window
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (_sdlWindow == nullptr)
		{
			fatalError("SDL Window could not be created!");
		}

		//Set up our OpenGL context
		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr)
		{
			fatalError("SDL_GL context could not be created!");
		}

		//Set up glew (optional but recommended)
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not initialize glew!");
		}

		//check opengl version
		std::printf("*** OpenGL Version: %s ***", glGetString(GL_VERSION));

		//Set the background color to black
		glClearColor(0.0f, 255.0f, 255.0f, 1.0f);

		//set VSYNC
		SDL_GL_SetSwapInterval(1);

		//enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	//Swap our buffer and draw everything to the screen!
	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(_sdlWindow);
	}

}