#pragma once
#include "Textures.h"
#include "SDL.h"
#include <stdio.h>
#include "SDL_image.h"

class PlayerHealthBar
{
public:
	PlayerHealthBar(const char* texturesheet, SDL_Renderer* ren, 
		int x, int y, int w, int h);
	~PlayerHealthBar();

	int dx;
	int xpos, ypos;
	int width, height, initialWidth;

	void update();
	void decreaseHealth();
	void increaseHealth(int bonus);
	void render();

	SDL_Texture* barTex;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	SDL_Renderer* renderer;
};

