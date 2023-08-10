#pragma once
#include "SDL.h"
#include <stdio.h>
#include "SDL_ttf.h"
#include<string>

class FontTextures
{
public:
	FontTextures(TTF_Font* font, SDL_Renderer* renderer, const std::string& text, int xx, int yy, int ww, int hh);
	~FontTextures();

	void setText(const std::string& text);

	void render();
	int x, y, w, h;

private:
	TTF_Font* font;
	SDL_Renderer* ren;
	SDL_Texture* fontTex;
};

