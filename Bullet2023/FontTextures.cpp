#include "FontTextures.h"


FontTextures::FontTextures(TTF_Font* font, SDL_Renderer* renderer, const std::string& text, int xx, int yy, int ww, int hh)
{
    x = xx;
    y = yy;
    w = ww;
    h = hh;
    ren = renderer;
    this->font = font;
}

void FontTextures::setText(const std::string& text)
{
    SDL_Surface* surfText = TTF_RenderText_Solid(font, text.c_str(), { 255, 255, 255 });
    fontTex = SDL_CreateTextureFromSurface(ren, surfText);
    SDL_FreeSurface(surfText);
}

void FontTextures::render()
{
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderCopy(ren, fontTex, NULL, &rect);
}

FontTextures::~FontTextures()
{
    SDL_DestroyTexture(fontTex);
}