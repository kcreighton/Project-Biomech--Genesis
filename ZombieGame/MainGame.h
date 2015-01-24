#pragma once

#include <Runengine2D/Window.h>
#include <Runengine2D/GLSLProgram.h>
#include <Runengine2D/Camera2D.h>
#include <Runengine2D/InputManager.h>

class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();

    /// Member Variables
    Runengine2D::Window _window; ///< The game window
    
    Runengine2D::GLSLProgram _textureProgram; ///< The shader program

    Runengine2D::InputManager _inputManager; ///< Handles input

    Runengine2D::Camera2D _camera; ///< Main Camera
};

