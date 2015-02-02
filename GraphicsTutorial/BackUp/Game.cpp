#include "Game.h"
#include "Errors.h"

#include "ImageLoader.h" 

#include <iostream>
#include <string>

//Constructor, just initializes private member variables
Game::Game() :
_window(nullptr),
_screenWidth(1024),
_screenHeight(768),
_gameState(GameState::PLAY),
_time(0.0f)
{

}

//Destructor
Game::~Game()
{

}

//This runs the game
void Game::run()
{
    initSystems();

    //Initialize our sprite. (temporary)
    _sprite.init(-1.0f, -1.0f, 2.0f, 2.0f);
	_playerTexture = ImageLoader::loadPNG("Textures/SpaceShooterRedux/PNG/ufoBlue.png");

    //This only returns when the game ends
    gameLoop();
}

//Initialize SDL and Opengl and whatever else we need
void Game::initSystems()
{
    //Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    //Open an SDL window
    _window = SDL_CreateWindow("ProjectBiomech:Genesis", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (_window == nullptr)
	{
		fatalError("SDL Window could not be created!");
    }

    //Set up our OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(_window);
    if (glContext == nullptr)
	{
        fatalError("SDL_GL context could not be created!");
    }

    //Set up glew (optional but recommended)
    GLenum error = glewInit();
    if (error != GLEW_OK)
	{
        fatalError("could not initialize glew!");
    }

    //Tell SDL that we want a double buffered window so we dont get any flickering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    //Set the background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    initShaders();
}


void Game::initShaders()
{
    _colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
    _colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
    _colorProgram.linkShaders();
}

//This is the main game loop for our program
void Game::gameLoop()
{

    //Will loop until we set _gameState to EXIT
    while (_gameState != GameState::EXIT)
	{
        processInput();
		_time += 0.05;
        drawGame();
    }
}

//Processes input with SDL
void Game::processInput()
{
    SDL_Event Event;

    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&Event))
	{
        switch (Event.type)
		{
            case SDL_QUIT:
                _gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                std::cout << Event.motion.x << " " << Event.motion.y << std::endl;
                break;
        }
    }
}

//Draws the game using the almighty OpenGL
void Game::drawGame() {

	//Set the base depth to 1.0
	glClearDepth(1.0);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Enable the shader
	_colorProgram.use();

	//We are using texture unit 0
	glActiveTexture(GL_TEXTURE0);
	//Bind the texture to texture unit 0
	glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	//Get the uniform location
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	//Tell the shader that the texture is in texture unit 0
	glUniform1i(textureLocation, 0);

	//Set the constantly changing time variable
	GLint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	//Draw our sprite!
	_sprite.draw();

	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//disable the shader
	_colorProgram.unuse();

	//Swap our buffer and draw everything to the screen!
	SDL_GL_SwapWindow(_window);
}