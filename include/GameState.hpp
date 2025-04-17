#pragma once
#include <SDL2/SDL.h>
#include <memory>

class GameState {
public:
    virtual ~GameState() = default;
    
    virtual void handleInput(SDL_Window* window, SDL_Renderer* renderer, const SDL_Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Window* window, SDL_Renderer* renderer) = 0;
    
    // Returns true if this state should be popped
    virtual bool shouldPop() const { return mShouldPop; }
    
    // Mark this state to be removed
    void requestStackPop() { mShouldPop = true; }
    
protected:
    bool mShouldPop = false;
}; 