#include "Enemy.hpp"
#include <cmath>
#include <algorithm>

Enemy::Enemy(float x, float y, float playerX, float playerY)
    : Entity(x, y, ENEMY_SPEED, ENEMY_HEALTH)
{
    mRadius = ENEMY_RADIUS;
    mColor = {255, 0, 0, 255}; // Red color
    
    // Update bounds to match radius
    mBounds.w = static_cast<int>(mRadius * 2);
    mBounds.h = static_cast<int>(mRadius * 2);
    
    // Initialize health bars
    mHealthBarBackground = {static_cast<int>(x - ENEMY_RADIUS), 
                           static_cast<int>(y - ENEMY_RADIUS - 15), 
                           static_cast<int>(ENEMY_RADIUS * 2), 5};
    
    mHealthBar = mHealthBarBackground;
    
    // Set initial direction toward player
    moveTowardsPlayer(playerX, playerY, 0.0f);
}

void Enemy::update(float deltaTime)
{
    // Apply velocity
    mX += mVelocityX * mSpeed * deltaTime;
    mY += mVelocityY * mSpeed * deltaTime;
    
    // Update bounds position
    mBounds.x = static_cast<int>(mX - mRadius);
    mBounds.y = static_cast<int>(mY - mRadius);
    
    // Update health bar position
    mHealthBarBackground.x = static_cast<int>(mX - ENEMY_RADIUS);
    mHealthBarBackground.y = static_cast<int>(mY - ENEMY_RADIUS - 15);
    
    mHealthBar.x = mHealthBarBackground.x;
    mHealthBar.y = mHealthBarBackground.y;
    mHealthBar.w = static_cast<int>((static_cast<float>(mHealth) / ENEMY_HEALTH) * (ENEMY_RADIUS * 2));
}

void Enemy::render(SDL_Renderer* renderer)
{
    // Draw enemy circle
    int resolution = 20; // Number of segments to draw the circle
    float angleStep = 2 * M_PI / resolution;
    
    for (int i = 0; i < resolution; i++) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;
        
        int x1 = static_cast<int>(mX + mRadius * std::cos(angle1));
        int y1 = static_cast<int>(mY + mRadius * std::sin(angle1));
        int x2 = static_cast<int>(mX + mRadius * std::cos(angle2));
        int y2 = static_cast<int>(mY + mRadius * std::sin(angle2));
        
        SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
    
    // Fill the circle (approximate)
    for (int offset = 0; offset < mRadius; offset++) {
        for (int i = 0; i < resolution; i++) {
            float angle1 = i * angleStep;
            float angle2 = (i + 1) * angleStep;
            
            int x1 = static_cast<int>(mX + offset * std::cos(angle1));
            int y1 = static_cast<int>(mY + offset * std::sin(angle1));
            int x2 = static_cast<int>(mX + offset * std::cos(angle2));
            int y2 = static_cast<int>(mY + offset * std::sin(angle2));
            
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    }
    
    // Draw health bar
    renderHealthBar(renderer);
}

void Enemy::moveTowardsPlayer(float playerX, float playerY, float deltaTime)
{
    // Calculate direction to player
    float dirX = playerX - mX;
    float dirY = playerY - mY;
    
    // Normalize the direction
    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length > 0) {
        mVelocityX = dirX / length;
        mVelocityY = dirY / length;
    } else {
        mVelocityX = 0;
        mVelocityY = 0;
    }
}

void Enemy::renderHealthBar(SDL_Renderer* renderer)
{
    // Draw background (empty health)
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(renderer, &mHealthBarBackground);
    
    // Draw filled health bar
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    SDL_RenderFillRect(renderer, &mHealthBar);
} 