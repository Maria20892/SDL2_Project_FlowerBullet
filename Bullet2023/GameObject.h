#pragma once
#include "SDL.h"
#include <stdio.h>
#include "SDL_image.h"
#include "Textures.h"

class GameObject
{
public:
	GameObject(const char* texturesheet, SDL_Renderer* ren, int x, int y, int speed, int w, int h, int healthy, int team, int W_W, int W_H);
	~GameObject();

	void update();
	void render(SDL_Renderer* renderer, int cameraX, int cameraY);
	bool inRectangle();
	void updateShapesTexture(const char* texturesheet);
	//void updateAnimation(int portionWidth, Uint32 deltaTime);
	void updateAnimation();
	void updatePlayerAnimation();
	void renderAnimation(int cameraX, int cameraY);
	SDL_Rect getSrcRect()const;
	

	float dx;
	float dy;

	//float initialDistance=0;
	//double angle;//stores current angle
	//float spiralAngle;


	float xpos, ypos;
	int health;
	int width, height;
	int side;
	
	int wind_w, w_height;

	int shootPattern;
	//float initialRadius = 5.0f;  // Initial radius of the spiral
	//float radiusMultiplier = 1.1f;

	SDL_Texture* objectTex;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	SDL_Renderer* renderer;

	int flag;//for blowing bullets
	float speed;
	double creationTime;

	int horisontalFrames;     // Total frames in the animation
	int verticalFrames=0;
};


