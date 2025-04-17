#pragma once
#include "GameState.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

class GameplayState : public GameState {
public:
    GameplayState();
    virtual ~GameplayState();
    
    virtual void handleInput(SDL_Window* window, SDL_Renderer* renderer, const SDL_Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void render(SDL_Window* window, SDL_Renderer* renderer) override;
    
private:
    void spawnEnemies();
    void handleCollisions();
    void cleanup();
    void firePlayerBullet(int mouseX, int mouseY);
    
    Player mPlayer;
    std::vector<Enemy> mEnemies;
    std::vector<Bullet> mBullets;
    
    // Game settings
    static constexpr int MAX_ENEMIES = 5;
    static constexpr float ENEMY_SPAWN_INTERVAL = 3.0f; // seconds
    
    Uint32 mLastEnemySpawn;
}; 