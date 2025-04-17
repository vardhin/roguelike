#include "Button.hpp"

Button::Button(const SDL_Rect& rect, const std::string& text, TTF_Font* font)
    : mState(ButtonState::Normal), mRect(rect), mText(text), mFont(font)
{
}

void Button::setCallback(std::function<void()> callback)
{
    mCallback = callback;
}

void Button::handleEvent(const SDL_Event& event, SDL_Window* window)
{
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        if (mouseX >= mRect.x && mouseX <= mRect.x + mRect.w &&
            mouseY >= mRect.y && mouseY <= mRect.y + mRect.h)
        {
            mState = ButtonState::Pressed;
        }
    }
    else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        if (mState == ButtonState::Pressed && 
            mouseX >= mRect.x && mouseX <= mRect.x + mRect.w &&
            mouseY >= mRect.y && mouseY <= mRect.y + mRect.h && 
            mCallback)
        {
            mCallback();
        }
        
        mState = ButtonState::Normal;
    }
}

void Button::update(int mouseX, int mouseY)
{
    if (mState != ButtonState::Pressed)
    {
        if (mouseX >= mRect.x && mouseX <= mRect.x + mRect.w &&
            mouseY >= mRect.y && mouseY <= mRect.y + mRect.h)
        {
            mState = ButtonState::Hover;
        }
        else
        {
            mState = ButtonState::Normal;
        }
    }
}

void Button::render(SDL_Renderer* renderer)
{
    // Draw button rectangle with appropriate color
    SDL_Color color;
    switch (mState) {
        case ButtonState::Hover:
            color = mHoverColor;
            break;
        case ButtonState::Pressed:
            color = mPressedColor;
            break;
        default:
            color = mNormalColor;
    }
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &mRect);
    
    // Draw outline
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &mRect);
    
    // Draw text
    if (mFont) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, mText.c_str(), mTextColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            
            if (textTexture) {
                // Center text
                SDL_Rect textRect;
                textRect.w = textSurface->w;
                textRect.h = textSurface->h;
                textRect.x = mRect.x + (mRect.w - textRect.w) / 2;
                textRect.y = mRect.y + (mRect.h - textRect.h) / 2;
                
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            
            SDL_FreeSurface(textSurface);
        }
    }
} 