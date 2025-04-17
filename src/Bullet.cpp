#include "Bullet.hpp"
#include <cmath>

Bullet::Bullet(float x, float y, float directionX, float directionY)
    : Entity(x, y, BULLET_SPEED, 1), mDirectionX(directionX), mDirectionY(directionY),
      mActive(true), mLifetime(0.0f)
{
    mRadius = BULLET_RADIUS;
    mColor = {255, 255, 0, 255}; // Yellow color
    
    // Update bounds to match radius
    mBounds.w = static_cast<int>(mRadius * 2);
    mBounds.h = static_cast<int>(mRadius * 2);
}

void Bullet::update(float deltaTime)
{
    // Update lifetime
    mLifetime += deltaTime;
    if (mLifetime >= MAX_LIFETIME) {
        mActive = false;
        return;
    }
    
    // Move in direction
    mX += mDirectionX * mSpeed * deltaTime;
    mY += mDirectionY * mSpeed * deltaTime;
    
    // Update bounds
    mBounds.x = static_cast<int>(mX - mRadius);
    mBounds.y = static_cast<int>(mY - mRadius);
}

void Bullet::render(SDL_Renderer* renderer)
{
    if (!mActive) return;
    
    // Draw bullet circle
    SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
    
    // Using a simpler circle drawing method for small bullets
    int resolution = 8; // Fewer segments for small bullets
    float angleStep = 2 * M_PI / resolution;
    
    for (int i = 0; i < resolution; i++) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;
        
        int x1 = static_cast<int>(mX + mRadius * std::cos(angle1));
        int y1 = static_cast<int>(mY + mRadius * std::sin(angle1));
        int x2 = static_cast<int>(mX + mRadius * std::cos(angle2));
        int y2 = static_cast<int>(mY + mRadius * std::sin(angle2));
        
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
    
    // Fill the circle (approximate)
    for (int i = 0; i < mRadius; i++) {
        for (int j = 0; j < resolution; j++) {
            float angle1 = j * angleStep;
            float angle2 = (j + 1) * angleStep;
            
            int x1 = static_cast<int>(mX + i * std::cos(angle1));
            int y1 = static_cast<int>(mY + i * std::sin(angle1));
            int x2 = static_cast<int>(mX + i * std::cos(angle2));
            int y2 = static_cast<int>(mY + i * std::sin(angle2));
            
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    }
} 