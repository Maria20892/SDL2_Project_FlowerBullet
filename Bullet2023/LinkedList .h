#pragma once
#include "Node.h"
#include "GameObject.h"
#include <stdlib.h> //C
#include <time.h> //C
#include "PlayerHealthBar.h"
#include <stdio.h>

class LinkedList 
{
public:
    Node* head; // The head of the linked list

    Node* getHead() const
    {
        return head;
    }


    LinkedList() : head(nullptr) {}

    ~LinkedList()
    {
        Node* current = head;
        while (current != nullptr) 
        {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    // Method to add a new object to the linked list
    void addObject(const GameObject& obj)
    {
        Node* newNode = new Node(obj);
        if (!head) 
        {
            head = newNode;
        }
        else 
        {
            Node* temp = head;
            while (temp->next) 
            {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // Method to update all objects in the linked list
    void updateAllObjects(bool enemy) 
    {
        Node* temp = head;
        while (temp) 
        {
            if (enemy == false)
            {
                temp->data.update();
                temp = temp->next;
            }
            else
            {
                temp->data.updateAnimation();
                temp = temp->next;
            }
        }
    }
    //update texture of the shapes
    void updateShapesTex(const char* texturesheet)
    {
        // Seed the random number generator with the current time
        srand((unsigned)time(NULL));


        // Define the range limits
        const int rangeMin = -1;
        const int rangeMax = 1;

        Node* temp = head;
        while (temp)
        {
            temp->data.updateShapesTexture(texturesheet);
            temp->data.dx = rangeMin + (rand() % (rangeMax - rangeMin + 1));
            temp->data.dy = rangeMin + (rand() % (rangeMax - rangeMin + 1));
            temp = temp->next;
        }
    }

    // Method to render all objects in the linked list
    void renderObjects(SDL_Renderer* renderer, int cameraX,int cameraY, bool movingEnemy)
    {
        Node* temp = head;
        while (temp) 
        {
            if (movingEnemy == 0)
            {
                temp->data.render(renderer, cameraX, cameraY);
                temp = temp->next;
            }
            else
            {
                temp->data.renderAnimation(cameraX, cameraY);
                temp = temp->next;
            }
        }
    }

    // Method to clean up memory and delete the linked list
    void deleteObjects() 
    {
        Node* temp = head;
        while (temp) 
        {
            Node* current = temp;
            temp = temp->next;
            delete current;
        }
        head = nullptr;
    }

    bool isEmpty() const 
    {
        return head == nullptr;
    }

    void shapesInRect()
    {
        Node* temp = head;
        Node* previous = nullptr;

        while (temp)
        {
            bool inRect = temp->data.inRectangle();

            if (inRect)
            {
                previous = temp;
                temp = temp->next;
            }
            else
            {
                temp->data.dx *= -1;
                temp->data.dy *= -1;
                temp = temp->next;
            }
        }
    }

    void bulletInRect()
    {
        Node* temp = head;
        Node* previous = nullptr;

        while (temp)
        {
            bool inRect = temp->data.inRectangle();

            if (inRect)
            {
                previous = temp;
                temp = temp->next;
            }
            else
            {
                Node* current = temp;
                temp = temp->next;

                if (current == head)
                {
                    head = current->next;
                }
                else
                {
                    previous->next = current->next;
                }

                delete current;
            }
        }
    }

    bool IsColliding(GameObject& bullet, GameObject& opponent)
    {
        return (std::max(bullet.xpos, opponent.xpos) < std::min((bullet.xpos + bullet.width), (opponent.xpos + opponent.width))) 
            && (std::max(bullet.ypos, opponent.ypos) < std::min((bullet.ypos + bullet.height), (opponent.ypos + opponent.height)));
    }

    void hasShapeCollision(GameObject& player, int& score, int& hitBoxEnemy, double& lastHitTime)
    {
        Node* temp = head;

        while (temp)
        {
            if (IsColliding(temp->data, player))
            {
                player.health -= temp->data.health;
                score -= 50;
                hitBoxEnemy++;
                temp = temp->next;
                lastHitTime = SDL_GetTicks() * 0.001;
            }
            else
            {
                // Move to the next bullet
                temp = temp->next;
            }

        }
    }

    void hasCollisionEnemies(GameObject& enemies, int& score, int& hitBoxPlayer)
    {
        Node* tempBul = head;
        Node* prevBul = nullptr;

        int playerCombo = 1;
        double hitTime = 0;
        double enterTime = SDL_GetTicks() * 0.001;

        while (tempBul)
        {
            if (IsColliding(tempBul->data, enemies))
            {
                enemies.health -= tempBul->data.health;
                hitBoxPlayer++;
                hitTime = SDL_GetTicks() * 0.001;
                if (enterTime - hitTime > 0.5)
                {
                    score += 95;
                    playerCombo = 1;
                }
                else
                {
                    playerCombo++;
                    score += (95 * playerCombo);
                }

                // Remove the bullet from the linked list
                if (prevBul == nullptr)
                {
                    // If the bullet to be removed is the head, update the head pointer
                    head = tempBul->next;
                    delete tempBul;
                    tempBul = head;
                }
                else
                {
                    // If the bullet is not the head, update the previous node's next pointer
                    prevBul->next = tempBul->next;
                    delete tempBul;
                    tempBul = prevBul->next;
                }
                break;
            }
            else
            {
                // Move to the next bullet
                prevBul = tempBul;
                tempBul = tempBul->next;
            }
        }
   
    }

    void hasCollisionPlayer(GameObject& opponent, int &score, int & hitBoxEnemy, double &lastHitTime, PlayerHealthBar& healthBar)
    {
        Node* temp = head;
        Node* prev = nullptr;
        double hitTime = 0;
        double enterTime = SDL_GetTicks() * 0.001;
        int enemyCombo = 1;

        while (temp)
        {
            if (IsColliding(temp->data, opponent))
            {
                opponent.health -= temp->data.health;
                healthBar.decreaseHealth();
                hitBoxEnemy++;
                lastHitTime = SDL_GetTicks() * 0.001;
                hitTime = SDL_GetTicks() * 0.001;
                if (enterTime - hitTime > 1.5)
                {
                    score -= 50;
                    enemyCombo = 1;
                }
                else
                {
                    enemyCombo++;
                    score -= (50 * enemyCombo);
                }
                // Remove the bullet from the linked list
                if (prev == nullptr)
                {
                    // If the bullet to be removed is the head, update the head pointer
                    head = temp->next;
                    delete temp;
                    temp = head;
                }
                else
                {
                    // If the bullet is not the head, update the previous node's next pointer
                    prev->next = temp->next;
                    delete temp;
                    temp = prev->next;
                }
                break;
            }
            else
            {
                // Move to the next bullet
                prev = temp;
                temp = temp->next;
            }
        }
    }

    void BonusCollision(GameObject& player, const int PLAYER_HEALTH, PlayerHealthBar& healthBar)
    {
        Node* temp = head;
        Node* prev = nullptr;
        int enemyCombo = 1;

        while (temp)
        {
            if (IsColliding(temp->data, player))
            {
                player.health += temp->data.health;
                healthBar.increaseHealth(temp->data.health);
                if (player.health > PLAYER_HEALTH)
                {
                    player.health = PLAYER_HEALTH;
                }

                // Remove the bullet from the linked list
                if (prev == nullptr)
                {
                    // If the bullet to be removed is the head, update the head pointer
                    head = temp->next;
                    delete temp;
                    temp = head;
                }
                else
                {
                    // If the bullet is not the head, update the previous node's next pointer
                    prev->next = temp->next;
                    delete temp;
                    temp = prev->next;
                }
                //break;
            }
            else
            {
                // Move to the next bullet
                prev = temp;
                temp = temp->next;
            }
        }
    }

    void IsAlive()
    {
        Node* temp = head;
        Node* previous = nullptr;

        while (temp)
        {
            if (temp->data.health > 0)
            {
                previous = temp;
                temp = temp->next;
            }
            else
            {
                Node* current = temp;
                temp = temp->next;

                if (current == head)
                {
                    head = current->next;
                }
                else
                {
                    previous->next = current->next;
                }

                delete current;
            }
        }
    }
};
