#pragma once
#include "Entity.hpp"
#include <SDL2/SDL.h>
#include <cmath>

class Enemy : public Entity {
public:
    Enemy(float x, float y, float playerX, float playerY);
    
    virtual void update(float deltaTime) override;
    virtual void render(SDL_Renderer* renderer) override;
    
    void moveTowardsPlayer(float playerX, float playerY, float deltaTime);
    
    // Constants
    static constexpr int ENEMY_HEALTH = 40;
    static constexpr float ENEMY_SPEED = 100.f;
    static constexpr float ENEMY_RADIUS = 15.f;
}; 