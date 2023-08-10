#pragma once
#include "SDL.h"
#include <stdio.h>
#include "SDL_image.h"


class Textures
{
public:
	static SDL_Texture* loadTexture(const char* fileName, SDL_Renderer* ren);


};

