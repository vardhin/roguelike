#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <iostream>
#include "GameState.hpp"
#include "StartMenu.hpp"
#include "GameplayState.hpp"

class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    bool createWindow();
    void changeState();

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    const float TIME_STEP = 1.0f/60.0f;
    const unsigned int DEFAULT_WIDTH = 1280;
    const unsigned int DEFAULT_HEIGHT = 720;
    
    std::unique_ptr<GameState> mCurrentState;
    bool mChangingState = false;
    std::unique_ptr<GameState> mNextState;
    bool mRunning = true;
}; 