#include "GameplayState.hpp"
#include <iostream>

GameplayState::GameplayState() : mPlayer(640, 360)
{
    mLastEnemySpawn = SDL_GetTicks();
}

GameplayState::~GameplayState()
{
    // No resources to clean up at this point
}

void GameplayState::handleInput(SDL_Window* window, SDL_Renderer* renderer, const SDL_Event& event)
{
    if (event.type == SDL_QUIT) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(0);
    }
    
    // Handle escape key to return to menu
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        requestStackPop();
    }
}

void GameplayState::update(float deltaTime)
{
    // Simple update for now
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    mPlayer.handleInput(keyState);
    mPlayer.update(deltaTime);
    
    // Just print a message for now showing we're in gameplay
    static bool messagePrinted = false;
    if (!messagePrinted) {
        std::cout << "In GameplayState - press ESC to return to menu" << std::endl;
        messagePrinted = true;
    }
}

void GameplayState::render(SDL_Window* window, SDL_Renderer* renderer)
{
    // Basic rendering
    mPlayer.render(renderer);
}

void GameplayState::spawnEnemies()
{
    // Stub for now
}

void GameplayState::handleCollisions()
{
    // Stub for now
}

void GameplayState::cleanup()
{
    // Stub for now
}

void GameplayState::firePlayerBullet(int mouseX, int mouseY)
{
    // Stub for now
}