#include "Entity.hpp"
#include <algorithm>

Entity::Entity(float x, float y, float speed, int health)
    : mX(x), mY(y), mVelocityX(0), mVelocityY(0), 
      mSpeed(speed), mHealth(health), mMaxHealth(health)
{
    // Initialize bounds rectangle
    mBounds.x = static_cast<int>(x - mRadius);
    mBounds.y = static_cast<int>(y - mRadius);
    mBounds.w = static_cast<int>(mRadius * 2);
    mBounds.h = static_cast<int>(mRadius * 2);
    
    // Default white color
    mColor = {255, 255, 255, 255};
}

void Entity::damage(int amount)
{
    mHealth = std::max(0, mHealth - amount);
} 