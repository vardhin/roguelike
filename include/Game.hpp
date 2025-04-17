#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

class Game {
public:
    Game();
    bool init();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    bool createWindow();

    sf::RenderWindow mWindow;
    const sf::Time TimePerFrame = sf::seconds(1.f/60.f);
    const unsigned int DEFAULT_WIDTH = 1280;
    const unsigned int DEFAULT_HEIGHT = 720;
}; 