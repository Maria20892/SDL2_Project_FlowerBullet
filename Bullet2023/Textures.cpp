#include "Textures.h"

SDL_Texture* Textures::loadTexture(const char* texture, SDL_Renderer* ren)
{
	SDL_Surface* tmpSurface = IMG_Load(texture);
	if (!tmpSurface)
	{
		fprintf(stderr, "IMG_Load error: %s\n", IMG_GetError());
	}
	SDL_SetColorKey(tmpSurface, SDL_TRUE, SDL_MapRGB(tmpSurface->format, 255, 255, 255));
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return tex;
}
