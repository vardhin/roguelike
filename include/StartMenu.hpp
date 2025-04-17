#pragma once
#include "GameState.hpp"
#include "Button.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <memory>
#include <string>

class StartMenu : public GameState {
public:
    StartMenu();
    virtual ~StartMenu();
    
    virtual void handleInput(SDL_Window* window, SDL_Renderer* renderer, const SDL_Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void render(SDL_Window* window, SDL_Renderer* renderer) override;
    
    // Flag to indicate the game should start
    bool shouldStartGame() const { return mStartGame; }
    
private:
    TTF_Font* mFont;
    SDL_Texture* mTitleTexture;
    SDL_Rect mTitleRect;
    std::vector<Button> mButtons;
    bool mStartGame = false;
    bool mExitGame = false;
}; 