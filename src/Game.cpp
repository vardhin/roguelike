#include "Game.hpp"

Game::Game()
{
}

bool Game::init()
{
    return createWindow();
}

bool Game::createWindow()
{
    // Try to create the window with default settings
    try {
        mWindow.create(sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT), "RogueLike Game", sf::Style::Close);
        
        // Try to enable VSync, but don't worry if it fails
        mWindow.setVerticalSyncEnabled(true);
        
        // Set fallback framerate limit if VSync is not available
        mWindow.setFramerateLimit(60);
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to create window: " << e.what() << std::endl;
        
        // Try to create window with minimum settings
        try {
            mWindow.create(sf::VideoMode(800, 600), "RogueLike Game", sf::Style::None);
            mWindow.setFramerateLimit(60);
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to create fallback window: " << e.what() << std::endl;
            return false;
        }
    }
}

void Game::run()
{
    if (!mWindow.isOpen()) {
        std::cerr << "Cannot run game without a window!" << std::endl;
        return;
    }

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }
        
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            mWindow.close();
            
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            mWindow.close();
    }
}

void Game::update(sf::Time deltaTime)
{
    // Update game logic here
}

void Game::render()
{
    mWindow.clear(sf::Color(20, 20, 20)); // Dark gray background
    // Draw game objects here
    mWindow.display();
} 