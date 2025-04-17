#pragma once
#include <SDL2/SDL.h>

class Entity {
public:
    Entity(float x, float y, float speed, int health);
    virtual ~Entity() = default;
    
    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
    
    float getX() const { return mX; }
    float getY() const { return mY; }
    void setPosition(float x, float y) { mX = x; mY = y; }
    
    float getSpeed() const { return mSpeed; }
    float getRadius() const { return mRadius; }
    
    int getHealth() const { return mHealth; }
    void damage(int amount);
    bool isAlive() const { return mHealth > 0; }
    
    SDL_Rect getBounds() const { return mBounds; }
    
protected:
    float mX, mY;
    float mVelocityX, mVelocityY;
    SDL_Rect mBounds;
    float mRadius;
    float mSpeed;
    int mHealth;
    int mMaxHealth;
    SDL_Color mColor;
}; 