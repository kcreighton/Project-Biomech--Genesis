#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <Runengine2D/Runengine2D.h> //open library
#include <Runengine2D/GLSLProgram.h>
#include <Runengine2D/GLTexture.h>
#include <Runengine2D/Sprite.h>
#include <Runengine2D/Window.h>
#include <Runengine2D/SpriteBatch.h>
#include <Runengine2D/Camera2D.h>
#include <Runengine2D/InputManager.h>
#include <Runengine2D/Timing.h>
#include "Bullet.h"

#include <vector>

enum class GameState {PLAY, EXIT};

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    void initSystems();
    void initShaders();
    void gameLoop();
    void processInput();
    void drawGame();

	Runengine2D::Window _window;
    int _screenWidth, _screenHeight;
	float _time, _maxFps, _fps;
    GameState _gameState;

	std::vector<Bullet> _bulletList;

	Runengine2D::FpsLimiter _fpsLimiter;
	Runengine2D::GLSLProgram _colorProgram;
	Runengine2D::SpriteBatch _spriteBatch;
	Runengine2D::InputManager _inputManager;
	Runengine2D::Camera2D _camera;
};

