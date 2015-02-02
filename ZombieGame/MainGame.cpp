#include "MainGame.h"

#include <Runengine2D/Runengine2D.h>

#include <SDL/SDL.h>
#include <iostream>

MainGame::MainGame() {
    // Empty
}

MainGame::~MainGame() {
    // IMPLEMENT THIS!
}

void MainGame::run() {
	_levelPointerList.push_back(new Level("Levels/level1.txt"));

	int a;
	std::cin >> a;
}

void MainGame::initSystems() {
    // IMPLEMENT THIS!
}

void MainGame::initShaders() {
    // Compile our color shader
    _textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColor");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();
}

void MainGame::gameLoop() {
   // IMPLEMENT THIS!
}

void MainGame::processInput() {
    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                // Exit the game here!
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.keyUp(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                _inputManager.keyDown(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
				_inputManager.keyUp(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
				_inputManager.keyDown(evnt.button.button);
                break;
        }
    }
}

void MainGame::drawGame()
{
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // IMPLEMENT THIS!
   
    // Swap our buffer and draw everything to the screen!
    _window.swapBuffer();
}