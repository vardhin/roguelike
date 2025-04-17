cmake_minimum_required(VERSION 3.10)
project(RogueLike)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify source files directly
set(SOURCES 
    src/main.cpp
    src/Game.cpp
    src/StartMenu.cpp
    src/Button.cpp
    src/Entity.cpp
    src/Player.cpp
    src/GameplayState.cpp
)

# Specify header files
set(HEADERS
    include/Game.hpp
    include/GameState.hpp
    include/StartMenu.hpp
    include/Button.hpp
    include/Entity.hpp
    include/Player.hpp
    include/GameplayState.hpp
)

# Find SDL2 packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# Link SDL2 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}) 