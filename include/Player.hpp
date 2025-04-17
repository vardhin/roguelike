#pragma once
#include "Entity.hpp"
#include <SDL2/SDL.h>

class Player : public Entity {
public:
    Player(float x, float y);
    
    virtual void update(float deltaTime) override;
    virtual void render(SDL_Renderer* renderer) override;
    
    void handleInput(const Uint8* keyState);
    void renderHealthBar(SDL_Renderer* renderer);
    void getAimDirection(int mouseX, int mouseY, float& dirX, float& dirY) const;
    
    // Constants
    static constexpr int PLAYER_HEALTH = 100;
    static constexpr float PLAYER_SPEED = 250.f;
    static constexpr float PLAYER_RADIUS = 20.f;
    static constexpr float FIRE_COOLDOWN = 0.25f; // in seconds
    
private:
    Uint32 mLastFireTime;
    SDL_Rect mHealthBar;
    SDL_Rect mHealthBarBackground;
}; 