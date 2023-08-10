#include "PlayerHealthBar.h"

PlayerHealthBar::PlayerHealthBar(const char* texturesheet, SDL_Renderer* ren,
	int x, int y, int w, int h)
{
	renderer = ren;
	barTex = Textures::loadTexture(texturesheet, renderer);
	xpos = x;
	ypos = y;
	dx = w / 50;
	width = w;
	initialWidth = w;
	height = h;
}

void PlayerHealthBar::decreaseHealth()
{
	width -= dx;
}

void PlayerHealthBar::increaseHealth(int bonus)
{
	width += bonus;
	if (width > initialWidth)
	{
		width = initialWidth;
	}
}

void PlayerHealthBar::update()
{
	srcRect.h = height;
	srcRect.w = width;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.h = srcRect.h;
	destRect.w = srcRect.w;
	destRect.x = xpos;
	destRect.y = ypos;

}

void PlayerHealthBar::render()
{
	SDL_RenderCopy(renderer, barTex, NULL, &destRect);
}

PlayerHealthBar::~PlayerHealthBar()
{
	SDL_DestroyTexture(barTex);
}
