#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <functional>
#include <string>

class Button {
public:
    Button(const SDL_Rect& rect, const std::string& text, TTF_Font* font);
    
    void setCallback(std::function<void()> callback);
    void handleEvent(const SDL_Event& event, SDL_Window* window);
    void update(int mouseX, int mouseY);
    void render(SDL_Renderer* renderer);
    
private:
    enum class ButtonState { Normal, Hover, Pressed };
    
    ButtonState mState;
    SDL_Rect mRect;
    std::string mText;
    TTF_Font* mFont;
    std::function<void()> mCallback;
    
    SDL_Color mNormalColor = {70, 70, 70, 255};
    SDL_Color mHoverColor = {100, 100, 100, 255};
    SDL_Color mPressedColor = {50, 50, 50, 255};
    SDL_Color mTextColor = {255, 255, 255, 255};
}; 