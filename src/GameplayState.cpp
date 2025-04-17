#include "GameplayState.hpp"
#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>

GameplayState::GameplayState() : mPlayer(640, 360), mScore(0)
{
    mLastEnemySpawn = SDL_GetTicks();
    mFont = TTF_OpenFont("assets/fonts/arial.ttf", 24);
    if (!mFont) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
}

GameplayState::~GameplayState()
{
    if (mFont) {
        TTF_CloseFont(mFont);
    }
    
    // Clean up any remaining bullets and enemies
    mBullets.clear();
    mEnemies.clear();
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
    
    // Handle mouse click to fire bullet
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        firePlayerBullet(mouseX, mouseY);
    }
}

void GameplayState::update(float deltaTime)
{
    // Update player
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    mPlayer.handleInput(keyState);
    mPlayer.update(deltaTime);
    
    // Spawn enemies
    spawnEnemies();
    
    // Update enemies
    for (auto& enemy : mEnemies) {
        enemy.moveTowardsPlayer(mPlayer.getX(), mPlayer.getY(), deltaTime);
        enemy.update(deltaTime);
    }
    
    // Update bullets
    for (auto it = mBullets.begin(); it != mBullets.end();) {
        it->update(deltaTime);
        
        // Check if bullet is still active
        if (!it->isActive()) {
            it = mBullets.erase(it);
        } else {
            ++it;
        }
    }
    
    // Handle collisions
    handleCollisions();
    
    // Remove dead enemies
    auto removeIt = std::remove_if(mEnemies.begin(), mEnemies.end(), 
                    [](const Enemy& enemy) { return !enemy.isAlive(); });
    mEnemies.erase(removeIt, mEnemies.end());
}

void GameplayState::render(SDL_Window* window, SDL_Renderer* renderer)
{
    // Render player
    mPlayer.render(renderer);
    
    // Render bullets
    for (auto& bullet : mBullets) {
        bullet.render(renderer);
    }
    
    // Render enemies
    for (auto& enemy : mEnemies) {
        enemy.render(renderer);
    }
    
    // Render score
    renderScore(renderer);
    
    // Render player health as text
    renderPlayerHealth(renderer);
}

void GameplayState::spawnEnemies()
{
    Uint32 currentTime = SDL_GetTicks();
    
    // Spawn an enemy every 2 seconds
    if (currentTime - mLastEnemySpawn > 2000) {
        mLastEnemySpawn = currentTime;
        
        // Random position at edge of screen
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> side(0, 3); // 0 = top, 1 = right, 2 = bottom, 3 = left
        
        int s = side(gen);
        float x = 0, y = 0;
        
        int screenWidth = 1280, screenHeight = 720;  // Default values
        SDL_Window* window = SDL_GL_GetCurrentWindow();
        if (window) {
            SDL_GetWindowSize(window, &screenWidth, &screenHeight);
        }
        
        std::uniform_int_distribution<> xDist(0, screenWidth);
        std::uniform_int_distribution<> yDist(0, screenHeight);
        
        if (s == 0) { // Top
            x = xDist(gen);
            y = -20;
        } else if (s == 1) { // Right
            x = screenWidth + 20;
            y = yDist(gen);
        } else if (s == 2) { // Bottom
            x = xDist(gen);
            y = screenHeight + 20;
        } else { // Left
            x = -20;
            y = yDist(gen);
        }
        
        mEnemies.emplace_back(x, y, mPlayer.getX(), mPlayer.getY());
    }
}

void GameplayState::handleCollisions()
{
    // Check bullet-enemy collisions
    for (auto& bullet : mBullets) {
        for (auto& enemy : mEnemies) {
            if (!enemy.isAlive() || !bullet.isActive()) continue;
            
            // Simple circle collision
            float dx = bullet.getX() - enemy.getX();
            float dy = bullet.getY() - enemy.getY();
            float distance = std::sqrt(dx*dx + dy*dy);
            
            if (distance < (5.0f + 15.0f)) {  // Use constants directly: BULLET_RADIUS + ENEMY_RADIUS
                enemy.damage(10);
                bullet.deactivate();
                
                if (!enemy.isAlive()) {
                    mScore += 10;
                }
                
                break; // Bullet hit something, no need to check other enemies
            }
        }
    }
    
    // Check player-enemy collisions
    for (auto& enemy : mEnemies) {
        if (!enemy.isAlive() || !mPlayer.isAlive()) continue;
        
        float dx = mPlayer.getX() - enemy.getX();
        float dy = mPlayer.getY() - enemy.getY();
        float distance = std::sqrt(dx*dx + dy*dy);
        
        if (distance < (20.0f + 15.0f)) {  // Use constants directly: PLAYER_RADIUS + ENEMY_RADIUS
            mPlayer.damage(1);
            enemy.damage(20);
            
            // Push player away from enemy
            if (distance > 0) {
                float pushX = dx / distance * 15.0f;
                float pushY = dy / distance * 15.0f;
                
                mPlayer.setPosition(mPlayer.getX() + pushX, mPlayer.getY() + pushY);
            }
        }
    }
}

void GameplayState::firePlayerBullet(int mouseX, int mouseY)
{
    float dirX, dirY;
    mPlayer.getAimDirection(mouseX, mouseY, dirX, dirY);
    
    // Create bullet at player position with direction toward mouse
    mBullets.emplace_back(mPlayer.getX(), mPlayer.getY(), dirX, dirY);
}

void GameplayState::renderScore(SDL_Renderer* renderer)
{
    if (!mFont) return;
    
    // Create score text
    std::string scoreText = "Score: " + std::to_string(mScore);
    SDL_Color textColor = {255, 255, 255, 255};
    
    SDL_Surface* surface = TTF_RenderText_Solid(mFont, scoreText.c_str(), textColor);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect destRect = {10, 10, surface->w, surface->h};
        
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
        
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void GameplayState::renderPlayerHealth(SDL_Renderer* renderer)
{
    if (!mFont) return;
    
    // Create health text
    std::string healthText = "Health: " + std::to_string(mPlayer.getHealth());
    SDL_Color textColor = {255, 255, 255, 255};
    
    SDL_Surface* surface = TTF_RenderText_Solid(mFont, healthText.c_str(), textColor);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect destRect = {10, 40, surface->w, surface->h};
        
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
        
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void GameplayState::cleanup()
{
    mBullets.clear();
    mEnemies.clear();
}