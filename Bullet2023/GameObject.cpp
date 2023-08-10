#include "GameObject.h"

GameObject::GameObject(const char* texturesheet, SDL_Renderer* ren, int x, int y, int speed, int w, int h, int healthy, int team, int W_W, int W_H)
{
	renderer = ren;
	objectTex = Textures::loadTexture(texturesheet, renderer);
	xpos = x;
	ypos = y;
	this->speed = speed;
	health = healthy;
	dx = 0;
	dy = 0;
	width = w;
	height = h;
	side = team;
	wind_w = W_W;
	w_height = W_H;
	shootPattern = 0;
	flag=0;
}

SDL_Rect GameObject::getSrcRect()const
{
	return srcRect;
}

void GameObject::updateAnimation()
{
	int frameDelay = 150;

	xpos += dx;
	ypos += dy;

	srcRect.h = height;
	srcRect.w = width;
	srcRect.x = width* static_cast<int>((SDL_GetTicks() / frameDelay) % horisontalFrames);
	srcRect.y = height * verticalFrames;

	destRect.h = srcRect.h;
	destRect.w = srcRect.w;
	destRect.x = xpos;
	destRect.y = ypos;
}

//void GameObject::renderAnimation()
//{
//	SDL_RenderCopy(renderer, objectTex, &srcRect, &destRect);
//}

void GameObject::renderAnimation(int cameraX, int cameraY)
{
	SDL_Rect adjustedDestRect = destRect;
	adjustedDestRect.x = destRect.x - cameraX;
	adjustedDestRect.y = destRect.y - cameraY;

	SDL_RenderCopy(renderer, objectTex, &srcRect, &adjustedDestRect);
}

void GameObject::update()
{

	xpos += dx; // Update the player's x-position based on the speed
	ypos += dy;

	srcRect.h = height;
	srcRect.w = width;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.h = srcRect.h;
	destRect.w = srcRect.w;
	destRect.x = xpos;
	destRect.y = ypos;
	
}


void GameObject::updateShapesTexture(const char* texturesheet)
{
	objectTex = Textures::loadTexture(texturesheet, renderer);
}


void GameObject::render(SDL_Renderer* renderer, int cameraX, int cameraY)
{
	SDL_Rect adjustedDestRect = destRect;
	adjustedDestRect.x = destRect.x - cameraX;
	adjustedDestRect.y = destRect.y - cameraY;

	SDL_RenderCopy(renderer, objectTex, &srcRect, &adjustedDestRect);
}


bool GameObject::inRectangle()
{

	if (xpos < 0)
	{
		return false;
	}

	if (ypos < 0)
	{
		return false;
	}

	if (xpos > wind_w - width)
	{
		return false;
	}

	if (ypos > w_height - height)
	{
		return false;
	}
	return true;
}

GameObject::~GameObject()
{
	SDL_DestroyTexture(objectTex);
}