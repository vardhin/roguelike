#include "Player.hpp"
#include <cmath>

Player::Player(float x, float y)
    : Entity(x, y, PLAYER_SPEED, PLAYER_HEALTH), mLastFireTime(0)
{
    mRadius = PLAYER_RADIUS;
    mColor = {0, 162, 232, 255}; // Blue color
    
    // Update bounds to match radius
    mBounds.w = static_cast<int>(mRadius * 2);
    mBounds.h = static_cast<int>(mRadius * 2);
    
    // Initialize health bars
    mHealthBarBackground = {static_cast<int>(x - PLAYER_RADIUS), 
                           static_cast<int>(y - PLAYER_RADIUS - 10), 
                           static_cast<int>(PLAYER_RADIUS * 2), 5};
    
    mHealthBar = mHealthBarBackground;
}

void Player::handleInput(const Uint8* keyState)
{
    mVelocityX = 0;
    mVelocityY = 0;
    
    if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]) {
        mVelocityY = -1;
    }
    if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN]) {
        mVelocityY = 1;
    }
    if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
        mVelocityX = -1;
    }
    if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
        mVelocityX = 1;
    }
    
    // Normalize diagonal movement
    if (mVelocityX != 0 && mVelocityY != 0) {
        float length = std::sqrt(mVelocityX * mVelocityX + mVelocityY * mVelocityY);
        mVelocityX /= length;
        mVelocityY /= length;
    }
}

void Player::update(float deltaTime)
{
    // Apply velocity
    mX += mVelocityX * mSpeed * deltaTime;
    mY += mVelocityY * mSpeed * deltaTime;
    
    // Update bounds position
    mBounds.x = static_cast<int>(mX - mRadius);
    mBounds.y = static_cast<int>(mY - mRadius);
    
    // Update health bar position
    mHealthBarBackground.x = static_cast<int>(mX - PLAYER_RADIUS);
    mHealthBarBackground.y = static_cast<int>(mY - PLAYER_RADIUS - 10);
    
    mHealthBar.x = mHealthBarBackground.x;
    mHealthBar.y = mHealthBarBackground.y;
    mHealthBar.w = static_cast<int>((static_cast<float>(mHealth) / PLAYER_HEALTH) * (PLAYER_RADIUS * 2));
}

void Player::render(SDL_Renderer* renderer)
{
    // Draw player circle
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
    
    // Draw health bar
    renderHealthBar(renderer);
}

void Player::renderHealthBar(SDL_Renderer* renderer)
{
    // Draw background (empty health)
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(renderer, &mHealthBarBackground);
    
    // Draw filled health bar
    SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255); // Green
    SDL_RenderFillRect(renderer, &mHealthBar);
}

void Player::getAimDirection(int mouseX, int mouseY, float& dirX, float& dirY) const
{
    // Calculate direction vector from player to mouse
    dirX = mouseX - mX;
    dirY = mouseY - mY;
    
    // Normalize the direction vector
    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length > 0) {
        dirX /= length;
        dirY /= length;
    } else {
        // Default to aiming right if mouse is at player position
        dirX = 1.0f;
        dirY = 0.0f;
    }
} 