#include "StartMenu.hpp"
#include <iostream>

StartMenu::StartMenu() : mFont(nullptr), mTitleTexture(nullptr)
{
    // Try different paths for the font
    const char* fontPaths[] = {
        "../assets/fonts/arial.ttf",
        "assets/fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", // Common Linux font
        "/usr/share/fonts/TTF/Arial.ttf"
    };
    
    for (const char* path : fontPaths) {
        mFont = TTF_OpenFont(path, 24);
        if (mFont) {
            std::cout << "Loaded font from: " << path << std::endl;
            break;
        }
    }
    
    if (!mFont) {
        std::cerr << "Failed to load any font! " << TTF_GetError() << std::endl;
    }
}

StartMenu::~StartMenu()
{
    if (mTitleTexture) {
        SDL_DestroyTexture(mTitleTexture);
    }
    
    if (mFont) {
        TTF_CloseFont(mFont);
    }
}

void StartMenu::handleInput(SDL_Window* window, SDL_Renderer* renderer, const SDL_Event& event)
{
    if (event.type == SDL_QUIT || 
        (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    
    // Handle button events
    for (auto& button : mButtons)
    {
        button.handleEvent(event, window);
    }
    
    // Check if we should exit
    if (mExitGame)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    
    // Request stack pop and indicate we want to start the game
    if (mStartGame)
    {
        requestStackPop();
    }
}

void StartMenu::update(float deltaTime)
{
    // Do nothing with deltaTime in menu
    (void)deltaTime;
    
    // Get mouse position
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    // Update buttons
    for (auto& button : mButtons)
    {
        button.update(mouseX, mouseY);
    }
}

void StartMenu::render(SDL_Window* window, SDL_Renderer* renderer)
{
    // Create title texture if not already created
    if (!mTitleTexture && mFont) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* titleSurface = TTF_RenderText_Blended(mFont, "Roguelike Game", textColor);
        
        if (titleSurface) {
            mTitleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
            
            // Set title position
            mTitleRect.w = titleSurface->w;
            mTitleRect.h = titleSurface->h;
            mTitleRect.x = (1280 - mTitleRect.w) / 2;
            mTitleRect.y = 200;
            
            SDL_FreeSurface(titleSurface);
        }
        
        // Create buttons if they don't exist yet
        if (mButtons.empty() && mFont) {
            // Play button
            SDL_Rect playRect = {540, 350, 200, 50};
            mButtons.emplace_back(playRect, "Play", mFont);
            mButtons.back().setCallback([this]() { mStartGame = true; });
            
            // Exit button
            SDL_Rect exitRect = {540, 450, 200, 50};
            mButtons.emplace_back(exitRect, "Exit", mFont);
            mButtons.back().setCallback([this]() { mExitGame = true; });
        }
    }
    
    // Draw title
    if (mTitleTexture) {
        SDL_RenderCopy(renderer, mTitleTexture, NULL, &mTitleRect);
    }
    
    // Draw buttons
    for (auto& button : mButtons)
    {
        button.render(renderer);
    }
} 