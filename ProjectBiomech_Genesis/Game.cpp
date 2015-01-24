#include "Game.h"
#include <Runengine2D/Errors.h>
#include <Runengine2D/ResourceManager.h>

#include <iostream>
#include <string>

//Constructor, just initializes private member variables
Game::Game() :
	_window(),
	_screenWidth(1024),
	_screenHeight(768),
	_time(0.0f),					//used in shader, esentually a frame counter
	_maxFps(60.0f),
	_fps(),
	_gameState(GameState::PLAY),
	_bulletList(),
	_fpsLimiter(),
	_colorProgram(),
	_spriteBatch(),
	_inputManager()

{
	_camera.init(_screenWidth, _screenHeight);
}

//Destructor
Game::~Game()
{

}

//This runs the game
void Game::run()
{
    initSystems();

    //This only returns when the game ends
    gameLoop();
}

//Initialize SDL and Opengl and whatever else we need
void Game::initSystems()
{
	Runengine2D::init();

	// WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };
	_window.create("ProjectBiomech: Genesis", _screenWidth, _screenHeight, 0);

	//load shaders
	initShaders();

	//loadrite batch
	_spriteBatch.init();


	_fpsLimiter.init(_maxFps);
}

void Game::initShaders()
{
    _colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
    _colorProgram.addAttribute("vertexPosition");
    _colorProgram.addAttribute("vertexColor");
    _colorProgram.addAttribute("vertexUV");
    _colorProgram.linkShaders();
}

//This is the main game loop for our program
void Game::gameLoop()
{

    //Will loop until we set _gameState to EXIT
	while (_gameState != GameState::EXIT)
	{
		_fpsLimiter.begin();

		processInput();
		_time += 0.1;

		_camera.update();

		//update all bullets
		for (int i = 0; i < _bulletList.size();)
		{
			if (_bulletList[i].update() == true)
			{
				_bulletList[i] = _bulletList.back();
				_bulletList.pop_back();
			}
			else
			{
				i++;
			}
		}

		drawGame();

		_fps = _fpsLimiter.end();

		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 600)
		{
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
	}
}

//Processes input with SDL
void Game::processInput()
{
    SDL_Event frameEvent;

	const float CAMERA_SPEED = 5.0f;
	const float CAMERA_SPEED_DIAGONAL = glm::sqrt(2 * CAMERA_SPEED); // optimise this // root is expensive
	const float SCALE_SPEED = 0.01f;

    //Will keep looping until there are no more events to process
	while (SDL_PollEvent(&frameEvent))
	{
        switch (frameEvent.type)
		{
            case SDL_QUIT:
                _gameState = GameState::EXIT;
                break;
			case SDL_KEYDOWN:
				_inputManager.keyDown(frameEvent.key.keysym.sym);
				break;
			case SDL_KEYUP:
				_inputManager.keyUp(frameEvent.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				_inputManager.keyDown(frameEvent.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				_inputManager.keyUp(frameEvent.button.button);
				break;
			case SDL_MOUSEMOTION:
				_inputManager.setMouseCoords(frameEvent.motion.x, frameEvent.motion.y);
//                std::cout << frameEvent.motion.x << " " << frameEvent.motion.y << std::endl;
				break;
        }
    }

	// controls

	if (_inputManager.isKeyPressed(SDLK_w))
	{
		if (_inputManager.isKeyPressed(SDLK_d))
		{
			_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED_DIAGONAL));
			_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED_DIAGONAL, 0.0f));
		}
		else if (_inputManager.isKeyPressed(SDLK_a))
		{
			_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED_DIAGONAL));
			_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED_DIAGONAL, 0.0f));
		}
		else
		{
			_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
		}
	}
	else if (_inputManager.isKeyPressed(SDLK_s))
	{
		if (_inputManager.isKeyPressed(SDLK_d))
		{
			_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED_DIAGONAL));
			_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED_DIAGONAL, 0.0f));
		}
		else if (_inputManager.isKeyPressed(SDLK_a))
		{
			_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED_DIAGONAL));
			_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED_DIAGONAL, 0.0f));
		}
		else
		{
			_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
		}

	}
	else if(_inputManager.isKeyPressed(SDLK_d))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	else if (_inputManager.isKeyPressed(SDLK_a))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	//make mouse wheel up and down later
	if (_inputManager.isKeyPressed(SDLK_q))
	{
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_e))
	{
		if ((_camera.getScale() - SCALE_SPEED) > 0)
		{
			_camera.setScale(_camera.getScale() - SCALE_SPEED);
		}
		else
		{
			_camera.setScale(0);
		}
	}
	//
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		_bulletList.emplace_back(playerPosition, direction, 5.00f, 1000);
	}
}

//Draws the game using the almighty OpenGL
//will clean this up later
void Game::drawGame()
{
	//Set the base depth to 1.0
	glClearDepth(1.0);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Enable the shader
	_colorProgram.use();

	//We are using texture unit 0
	glActiveTexture(GL_TEXTURE0);
	//Get the uniform location
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	//Tell the shader that the texture is in texture unit 0
	glUniform1i(textureLocation, 0);

	//Set the constantly changing time variable
	//GLint timeLocation = _colorProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, _time);

	//Set the camera matrix
	GLint pLocation = _colorProgram.getUniformLocation("p");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 500.0f, 500.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Runengine2D::GLTexture texture = Runengine2D::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	Runengine2D::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

	for (int i = 0; i < _bulletList.size(); i++)
	{
		_bulletList[i].draw(_spriteBatch);
	}

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//disable the shader
	_colorProgram.unuse();

	//Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}

