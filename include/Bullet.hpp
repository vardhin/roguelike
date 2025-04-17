#pragma once
#include "Entity.hpp"
#include <SDL2/SDL.h>

class Bullet : public Entity {
public:
    Bullet(float x, float y, float directionX, float directionY);
    
    virtual void update(float deltaTime) override;
    virtual void render(SDL_Renderer* renderer) override;
    
    bool isActive() const { return mActive; }
    void deactivate() { mActive = false; }
    float getRadius() const { return mRadius; }
    
    // Constants
    static constexpr float BULLET_SPEED = 600.f;
    static constexpr float BULLET_RADIUS = 5.f;
    static constexpr int BULLET_DAMAGE = 20;
    static constexpr float MAX_LIFETIME = 2.0f; // seconds
    
private:
    float mDirectionX, mDirectionY;
    bool mActive;
    float mLifetime;
}; 