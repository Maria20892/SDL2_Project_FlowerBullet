#pragma once
#include "GameObject.h"


#ifndef SPIRALOBJECT_H
#define SPIRALOBJECT_H



class Bullet : public GameObject {
public:
    using GameObject::GameObject;

    // Override the update function for spiral movement
    void updateBullet();
};

#endif // SPIRALOBJECT_H

