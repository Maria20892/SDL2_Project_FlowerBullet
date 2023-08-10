#pragma once
#include "SDL.h"
#include <stdio.h>
#include "SDL_image.h"
#include "Textures.h"
#include "GameObject.h"


class Node {
public:
    GameObject data;
    Node* next;

    Node(const GameObject& obj) : data(obj), next(nullptr) {}
};