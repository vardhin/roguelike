#include "Game.hpp"

Game::Game() : mWindow(nullptr), mRenderer(nullptr)
{
}

Game::~Game()
{
    if (mRenderer) {
        SDL_DestroyRenderer(mRenderer);
    }
    if (mWindow) {
        SDL_DestroyWindow(mWindow);
    }
    
    // Quit SDL subsystems
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

bool Game::init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    
    if (!createWindow()) {
        return false;
    }
    
    // Start with the menu state
    mCurrentState = std::make_unique<StartMenu>();
    
    return true;
}

bool Game::createWindow()
{
    // Try to create the window with default settings
    mWindow = SDL_CreateWindow("RogueLike Game", 
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                               DEFAULT_WIDTH, DEFAULT_HEIGHT, 
                               SDL_WINDOW_SHOWN);
    
    if (!mWindow) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        
        // Try to create window with minimum settings
        mWindow = SDL_CreateWindow("RogueLike Game", 
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                  800, 600, 
                                  SDL_WINDOW_SHOWN);
        
        if (!mWindow) {
            std::cerr << "Failed to create fallback window: " << SDL_GetError() << std::endl;
            return false;
        }
    }
    
    // Create renderer for window
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (!mRenderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        // Try software renderer
        mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_SOFTWARE);
        
        if (!mRenderer) {
            std::cerr << "Failed to create software renderer: " << SDL_GetError() << std::endl;
            return false;
        }
    }
    
    return true;
}

void Game::run()
{
    if (!mWindow || !mRenderer) {
        std::cerr << "Cannot run game without a window and renderer!" << std::endl;
        return;
    }

    Uint32 lastTime = SDL_GetTicks();
    float accumulator = 0.0f;

    while (mRunning)
    {
        // Calculate delta time
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f; // Convert to seconds
        lastTime = currentTime;
        
        accumulator += deltaTime;
        
        processEvents();
        
        while (accumulator >= TIME_STEP)
        {
            update(TIME_STEP);
            accumulator -= TIME_STEP;
        }
        
        render();
        
        // Handle state changes after render
        changeState();
    }
}

void Game::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            mRunning = false;
        }
        
        // Let the current state handle events
        if (mCurrentState) {
            mCurrentState->handleInput(mWindow, mRenderer, event);
        }
    }
}

void Game::update(float deltaTime)
{
    // Update current state
    if (mCurrentState) {
        mCurrentState->update(deltaTime);
        
        // Check if we need to change state
        if (mCurrentState->shouldPop()) {
            if (StartMenu* startMenu = dynamic_cast<StartMenu*>(mCurrentState.get())) {
                if (startMenu->shouldStartGame()) {
                    // Switch to gameplay state
                    mChangingState = true;
                    mNextState = std::make_unique<GameplayState>();
                }
            } else {
                // Switch back to menu
                mChangingState = true;
                mNextState = std::make_unique<StartMenu>();
            }
        }
    }
}

void Game::render()
{
    // Clear screen
    SDL_SetRenderDrawColor(mRenderer, 20, 20, 20, 255); // Dark gray background
    SDL_RenderClear(mRenderer);
    
    // Let the current state render
    if (mCurrentState) {
        mCurrentState->render(mWindow, mRenderer);
    }
    
    // Update screen
    SDL_RenderPresent(mRenderer);
}

void Game::changeState()
{
    if (mChangingState && mNextState) {
        mCurrentState = std::move(mNextState);
        mChangingState = false;
    }
} 