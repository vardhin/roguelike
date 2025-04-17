#include "Game.hpp"
#include <iostream>

int main()
{
    try
    {
        Game game;
        if (!game.init()) {
            std::cerr << "Failed to initialize game" << std::endl;
            return 1;
        }
        game.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "\nEXCEPTION: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 