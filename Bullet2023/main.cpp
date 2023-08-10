#include "SDL.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <vector>
#include "Textures.h"
#include "GameObject.h"
#include "Node.h"
#include "LinkedList .h"
#include "SDL_ttf.h"
#include <random>
#include<string>
#include "FontTextures.h"
#include "PlayerHealthBar.h"
#include <fstream>
#include <stdlib.h>
#include <time.h>

const int WINDOW_WIDTH = 2400;
const int WINDOW_HEIGHT = 1600;
const int VISIBLE_WIDTH = 1200;
const int VISIBLE_HEIGHT = 800;
const int PLAYERS_SPEED = 5;
const int PL_BULLET_SPEED = 10;
const int EN_BULLET_SPEED = 4;
const int ENEMY_SPEED = 5;
const int SMALL_BULLET_POWER = 1;
const int BIG_BULLET_POWER = 3;
const int SMALL_BULLET_SIZE = 20;
const int BIG_BULLET_SIZE = 30;
const int ENEMY_HEALTH = 10;
const int PLAYER_HEALTH = 50;
const int BONUS_HEALTH = 5;
const int ENEMY_TEAM = 1;
const int PLAYER_TEAM = 0;
const int OBJECT_SIZE = 61;
const int PLAYER_WIDTH= 32;
const int PLAYER_HEIGHT = 38;
#define MINX 100
#define MAX_X WINDOW_WIDTH-OBJECT_SIZE
#define MINY 0
#define MAX_Y WINDOW_HEIGHT-OBJECT_SIZE
#define NUMBER_OF_ENEMIES 2
#define HEALTH_BAR_WIDTH 300
#define HEALTH_BAR_HEIGHT 20
#define LEVELS 3

void cleanupGame(GameObject*& player, LinkedList& enemies, LinkedList& bullets, LinkedList& playerBullets,
    LinkedList& blowingBullets, LinkedList& bonuses, LinkedList& shapes)
{
    if (!enemies.isEmpty()) {
        enemies.deleteObjects();
    }

    if (!bullets.isEmpty()) {
        bullets.deleteObjects();
    }

    if (!playerBullets.isEmpty()) {
        playerBullets.deleteObjects();
    }
    if (!blowingBullets.isEmpty()) {
        blowingBullets.deleteObjects();
    }
    if (!shapes.isEmpty()) {
        shapes.deleteObjects();
    }
    if (!bonuses.isEmpty()) {
        bonuses.deleteObjects();
    }

    delete player;
    player = nullptr;
}

void saveScoreToFile(const std::string& nickname, int score) 
{
    std::ofstream scoreFile("highscores.txt", std::ios::app);
    if (scoreFile.is_open()) {
        scoreFile << nickname << " " << score << "\n";
        scoreFile.close();
    }
    else {
        fprintf(stderr, "Error opening the score file %s\n", IMG_GetError());
        //std::cerr << "Error opening the score file.\n";
    }
}

struct ScoreEntry {
    std::string nickname;
    int score;
};

bool compareScores(const ScoreEntry& a, const ScoreEntry& b) {
    return a.score > b.score;
}

void displayHighScores(TTF_Font* font, SDL_Renderer* renderer, SDL_Window* window, GameObject*& player, 
    LinkedList& enemies, LinkedList& bullets, LinkedList& playerBullets,
    LinkedList& blowingBullets, LinkedList& bonuses, LinkedList& shapes)
{
    FontTextures* scoreTitle = new FontTextures(font, renderer, "SCORES:", VISIBLE_WIDTH / 2 - 100, 100, 100, 25);;
    FontTextures* menuPrompt = new FontTextures(font, renderer, "PRESS SPACE TO GO TO MENU", VISIBLE_WIDTH / 2 - 150, VISIBLE_HEIGHT - 150, 300, 25);
    menuPrompt->setText("PRESS SPACE TO GO TO MENU");
    bool chooseOption = false;

    std::vector<FontTextures*> scoreTextObjects; // To store score text objects


    std::ifstream scoreFile("highscores.txt");
    if (scoreFile.is_open()) {
        std::vector<ScoreEntry> scores;
        ScoreEntry entry;
        while (scoreFile >> entry.nickname >> entry.score) 
        {
            scores.push_back(entry);
        }
        scoreFile.close();

        std::sort(scores.begin(), scores.end(), compareScores);
        

        int page = 0;
        const int scoresPerPage = 10;
        int maxPages = (scores.size() + scoresPerPage - 1) / scoresPerPage;



        while (chooseOption==false)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    delete scoreTitle;
                    scoreTitle = nullptr;
                    //delete temp;
                    //temp = nullptr;
                    delete menuPrompt;
                    menuPrompt = nullptr;
                    cleanupGame(player, enemies, bullets, playerBullets,
                        blowingBullets, bonuses, shapes);
                    TTF_CloseFont(font);
                    TTF_Quit();
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    printf("Game cleaned\n");
                    exit(0);
                }
                else if (event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_SPACE:
                        chooseOption = true;
                        break;
                    case SDLK_LEFT:
                        page--;
                        if (page < 0)
                            page = 0;
                        break;
                    case SDLK_RIGHT:
                        page++;
                        if (page >= maxPages)
                        {
                            page = maxPages-1;
                        }
                        break;
                    }
                }
            }
            scoreTitle->setText("SCORES:");
            SDL_RenderClear(renderer);
            scoreTitle->render();

            for (FontTextures* textObj : scoreTextObjects) {
                delete textObj;
            }
            scoreTextObjects.clear();

            for (size_t i = 0; i < scoresPerPage; i++)
            {
                if (i + scoresPerPage * page < scores.size())
                {
                    const auto& score = scores[i + 10 * page];
                   // temp = new FontTextures(font, renderer, "->", scoreTitle->x, scoreTitle->y + 35 * (i+1), 150, 20);
                    FontTextures* temp = new FontTextures(font, renderer, "->", 
                        scoreTitle->x, scoreTitle->y + 35 * (i + 1), 150, 20);
                    std::string scoreTextstr = score.nickname + "_____" + std::to_string(score.score);;
                    temp->setText(scoreTextstr);
                    temp->render();
                    scoreTextObjects.push_back(temp); // Store for later cleanup
                }
            }
            menuPrompt->render();
            SDL_RenderPresent(renderer);
        }  
    }
    else 
    {
        fprintf(stderr, "Error opening the score file %s\n", IMG_GetError());
        //std::cerr << "Error opening the score file.\n";
    }

    delete scoreTitle;
    delete menuPrompt;
}

void menu(TTF_Font* font, SDL_Renderer* renderer, SDL_Window* window, GameObject*& player, LinkedList& enemies, LinkedList& bullets, LinkedList& playerBullets,
    LinkedList& blowingBullets, LinkedList& bonuses, LinkedList& shapes)
{
    bool chooseOption = false;
    FontTextures* newGame = new FontTextures(font, renderer, "NEW GAME", VISIBLE_WIDTH / 2 - 150, VISIBLE_HEIGHT / 2 - 30, 200, 35);
    FontTextures* scores = new FontTextures(font, renderer, "SCORES", VISIBLE_WIDTH / 2 - 150, newGame->y +50, 120, 35);
    FontTextures* quitGame = new FontTextures(font, renderer, "QUIT", VISIBLE_WIDTH / 2 - 150, scores->y + 50, 100, 35);
    FontTextures* menuPoint = new FontTextures(font, renderer, "->", newGame->x - 50, newGame->y, 30, 30);

    newGame->setText("NEW GAME");
    scores->setText("SCORES");
    quitGame->setText("QUIT");
    menuPoint->setText("->");

    while (chooseOption == false)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                // Handle quit event (e.g., pressing the close button)
                delete newGame;
                delete quitGame;
                delete scores;
                delete menuPoint;
                cleanupGame(player, enemies, bullets, playerBullets,
                    blowingBullets, bonuses, shapes);
                TTF_CloseFont(font);
                TTF_Quit();
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                printf("Game cleaned\n");
                exit(0);
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    if (menuPoint->y == scores->y)
                    {
                        menuPoint->y = newGame->y;
                    }
                    else
                    {
                        menuPoint->y = scores->y;
                    }
                    break;
                case SDLK_DOWN:
                    if (menuPoint->y == newGame->y)
                    {
                        menuPoint->y = scores->y;
                    }
                    else
                    {
                        menuPoint->y = quitGame->y;
                    }
                    break;
                case SDLK_RETURN:
                    if (menuPoint->y == quitGame->y)
                    {
                        delete newGame;
                        delete quitGame;
                        delete scores;
                        delete menuPoint;
                        cleanupGame(player, enemies, bullets, playerBullets,
                            blowingBullets, bonuses, shapes);
                        TTF_CloseFont(font);
                        TTF_Quit();
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        printf("Game cleaned\n");
                        exit(0);
                    }
                    else if(menuPoint->y == newGame->y)
                    {
                        chooseOption = true;
                    }
                    else
                    {
                        displayHighScores(font,renderer, window, player, enemies, bullets, playerBullets,
                            blowingBullets, bonuses, shapes);
                    }
                    break;
                }
            }
        }
        SDL_RenderClear(renderer);
        newGame->render();
        scores->render();
        quitGame->render();
        menuPoint->render();
        SDL_RenderPresent(renderer);
    }

    // Clean up
    delete newGame;
    newGame = nullptr;
    delete menuPoint;
    menuPoint = nullptr;
    delete quitGame;
    quitGame = nullptr;
}


void gameOvermenu(TTF_Font* font, SDL_Renderer* renderer, SDL_Window* window, int score, GameObject*& player, LinkedList& enemies, LinkedList& bullets, LinkedList& playerBullets,
    LinkedList& blowingBullets, LinkedList& bonuses, LinkedList& shapes)
{
    FontTextures* introText = new FontTextures(font, renderer, "CONGRATULATIONS YOU WON!", VISIBLE_WIDTH / 2 - 175, 70, 400, 50);
    FontTextures* introTextScore = new FontTextures(font, renderer, "YOUR SCORE IS: ", VISIBLE_WIDTH / 2 - 80, introText->y + 60, 200, 35);
    FontTextures* enterText = new FontTextures(font, renderer, "ENTER YOUR NAME AND PRESS \"ENTER\":", VISIBLE_WIDTH / 2 - 165, introTextScore->y+50, 400, 35);

    introText->setText("CONGRATULATIONS YOU WON!");
    std::string scoreTextstr = "YOUR SCORE IS: " + std::to_string(score);
    introTextScore->setText(scoreTextstr);
    enterText->setText("ENTER YOUR NAME AND PRESS \"ENTER\":");

    bool userInputdone = false;
    std::string playerName = ""; // To store user's input
    FontTextures* nameInput = new FontTextures(font, renderer, playerName, VISIBLE_WIDTH / 2 - 80, enterText->y + 50, 200, 35);
    int counter = 0;

    while (userInputdone == false)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                delete introText;
                delete introTextScore;
                delete enterText;
                delete nameInput;
                cleanupGame(player, enemies, bullets, playerBullets,
                    blowingBullets, bonuses, shapes);
                TTF_CloseFont(font);
                TTF_Quit();
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                printf("Game cleaned\n");
                exit(0);
                userInputdone = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    if (!playerName.empty())
                    {
                        saveScoreToFile(playerName, score);
                        userInputdone = true;
                    }
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE && !playerName.empty())
                {
                    playerName.pop_back(); 
                }
            }
            else if (event.type == SDL_TEXTINPUT)
            {  
                playerName += event.text.text;
                counter++;
                nameInput->w = 20 * counter; 
            }
        }

        nameInput->setText(playerName);
        SDL_RenderClear(renderer);
        introText->render();
        introTextScore->render();
        enterText->render();
        nameInput->render();
        SDL_RenderPresent(renderer);
    }
    // Clean up
    delete introText;
    introText = nullptr;
    delete introTextScore;
    introTextScore = nullptr;
    delete enterText;
    enterText = nullptr;
    delete nameInput;
    nameInput = nullptr;
}

int getRandomXPosition() {
    int randomX = rand() % (MAX_X - MINX + 1) + MINX;
    return randomX;
}

// Function to generate a random Y position
int getRandomYPosition() {
    int randomY = rand() % (MAX_Y - MINY + 1) + MINY;
    return randomY;
}

void calcSlope(int x1, int y1, int x2, int y2, float* dx, float* dy)
{
    int steps = std::max(abs(x1 - x2), abs(y1 - y2));

    if (steps == 0)
    {
        *dx = *dy = 0;
        return;
    }

    *dx = (x1 - x2);
    *dx /= steps;

    *dy = (y1 - y2);
    *dy /= steps;
}

void DirectedBullet(GameObject* enemy, LinkedList& bullets, SDL_Renderer* renderer, GameObject* player)
{
    GameObject* bullet = new GameObject("assets/big_bullet_enemy.png", renderer, enemy->xpos + enemy->width / 2.0, enemy->ypos + enemy->height / 2.0,
        EN_BULLET_SPEED, BIG_BULLET_SIZE, BIG_BULLET_SIZE, BIG_BULLET_POWER, ENEMY_TEAM, WINDOW_WIDTH, WINDOW_HEIGHT);

    bullet->shootPattern = enemy->shootPattern;

    calcSlope(player->xpos + (player->width / 2), player->ypos + (player->height / 2), enemy->xpos, enemy->ypos, &bullet->dx, &bullet->dy);

    bullet->dx *= EN_BULLET_SPEED;
    bullet->dy *= EN_BULLET_SPEED;

    bullets.addObject(*bullet);
}

void circleBullets(GameObject* enemy, LinkedList &bullets, SDL_Renderer* renderer, int big, int numBullets)
{
    int N = 10;
    int whichBullet;
    int bulletSize;
    int bulletPower;
    const char* texturesheet;

    if (big == 1)
    {
        whichBullet = 1;//small bullet
    }
    else if (big == 2)
    {
        whichBullet = 8;//big bullet
    }
    else
    {
        whichBullet = rand() % N; //random size bullet
    }

    if (whichBullet > 7)
    {
        bulletSize = BIG_BULLET_SIZE;
        bulletPower = BIG_BULLET_POWER;
        texturesheet = "assets/big_bullet_enemy.png";
    }
    else
    {
        bulletSize = SMALL_BULLET_SIZE;
        bulletPower = SMALL_BULLET_POWER;
        texturesheet = "assets/bullet_enemy.png";
    }
 
    float angleIncrement = 360.0 / numBullets; // Angle increment between bullets

    for (int i = 0; i < numBullets; ++i)
    {
        GameObject* bullet = new GameObject(texturesheet, renderer, enemy->xpos + enemy->width / 2.0, enemy->ypos + enemy->height / 2.0,
            EN_BULLET_SPEED, bulletSize, bulletSize, bulletPower, ENEMY_TEAM, WINDOW_WIDTH, WINDOW_HEIGHT);
        
        bullet->shootPattern = enemy->shootPattern;
        // Calculate the angle for the current bullet
        float angle = i * angleIncrement;
        // Convert the angle from degrees to radians
        float radianAngle = angle * (M_PI / 180.0);

        // Calculate the direction for the bullet using trigonometry
        bullet->dx = bullet->speed * cos(radianAngle);
        bullet->dy = bullet->speed * sin(radianAngle);

        bullets.addObject(*bullet);
    }   
}

void initPlayer(SDL_Renderer * renderer, GameObject * &player, LinkedList& bullets) 
{
    player = new GameObject("assets/MovingPlayer.png", renderer, 200, 200, PLAYERS_SPEED,
        PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_HEALTH, PLAYER_TEAM, WINDOW_WIDTH, WINDOW_HEIGHT);
    player->horisontalFrames = 4;
    player->verticalFrames = 4;

}

void initBonus(SDL_Renderer* renderer, LinkedList& bonuses) 
{
    int randomX;
    int randomY;
    GameObject* bonus;
    int N = 2;
    int amount = (rand() % N)+1;
    for (int i = 0; i < amount; i++)
    {
        randomX = getRandomXPosition();
        randomY = getRandomYPosition();
        bonus = new GameObject("assets/big_bullet_pl.png", renderer, randomX, randomY, PLAYERS_SPEED,
            OBJECT_SIZE/2, OBJECT_SIZE/2, BONUS_HEALTH, PLAYER_TEAM, WINDOW_WIDTH, WINDOW_HEIGHT);
        bonuses.addObject(*bonus);
        bonus->creationTime = SDL_GetTicks() * 0.001;
    }
}

void initShapes(SDL_Renderer* renderer, LinkedList& shapes) {
    int randomX;
    int randomY;
    GameObject* shape;

    for (int i = 0; i < 80; i++)
    {
        randomX = getRandomXPosition();
        randomY = getRandomYPosition();
        shape = new GameObject("assets/Shape_enemy.png", renderer, randomX, randomY, EN_BULLET_SPEED,
            OBJECT_SIZE * 2, OBJECT_SIZE * 2, BIG_BULLET_POWER, ENEMY_TEAM, WINDOW_WIDTH, WINDOW_HEIGHT);
        shape->shootPattern = 5;
        shapes.addObject(*shape);
        shape->creationTime = SDL_GetTicks() * 0.001;
    }  
}

void initEnemies(SDL_Renderer* renderer, LinkedList& enemies, int pattern, bool isMoving) {
    int randomX;
    int randomY;
    GameObject* enemy;
    int direction;

    if (!isMoving)
    {
        randomX = getRandomXPosition(); 
        randomY = getRandomYPosition();
        enemy = new GameObject("assets/enemy1.png", renderer, randomX, randomY, ENEMY_SPEED,
            OBJECT_SIZE, OBJECT_SIZE, ENEMY_HEALTH, ENEMY_TEAM, WINDOW_WIDTH, WINDOW_HEIGHT);
        enemy->shootPattern = pattern;
        enemies.addObject(*enemy);
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            direction = (i % 2 == 0) ? -1 : 1;
            randomX = (i % 2 == 0) ? WINDOW_WIDTH-OBJECT_SIZE : 0;;
            randomY = getRandomYPosition();
           
            enemy = new GameObject("assets/animated_enemy1.png", renderer, randomX, randomY, ENEMY_SPEED,
                OBJECT_SIZE, OBJECT_SIZE, ENEMY_HEALTH, ENEMY_TEAM, WINDOW_WIDTH, WINDOW_HEIGHT);

            enemy->dx = direction*ENEMY_SPEED;
            enemy->shootPattern = pattern;
            enemy->horisontalFrames = 7;

            enemies.addObject(*enemy);
        } 
    }
}

void initializeGame(SDL_Renderer* renderer, GameObject*& player, GameObject*& enemy, LinkedList& bullets, LinkedList& enemies, 
    LinkedList& playerBullets, LinkedList& blowingBul, LinkedList& bonuses, LinkedList& shapes,
    bool &isMoving, int &shootingPattern, int &score, double &startTime, int &levels, int &hitBoxPlayer, int &hitBoxEnemy, bool &isRunning)
{
    cleanupGame(player, enemies, bullets, playerBullets, blowingBul, bonuses, shapes);
    initPlayer(renderer, player, playerBullets);
    isMoving = false;
    shootingPattern = 0;
    initEnemies(renderer, enemies, shootingPattern, isMoving);
    score = 0;
    startTime = SDL_GetTicks() * 0.001;
    levels = LEVELS;
    hitBoxPlayer = 0;
    hitBoxEnemy = 0;
    isRunning = true;
}

void capFrameRate(long* then, float* remainder);

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////         MAIN      /////////////////////////

int main(int argc, char* argv[])
{
    SDL_Rect camera = { 0, 0, VISIBLE_WIDTH, VISIBLE_HEIGHT }; // Camera's position and size x,y,w,h

    long then;
    float remainder;

    then = SDL_GetTicks();
    remainder = 0;
    int startBigBullet = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    else
    {
        printf("SDL video sys is ready to go\n");
    }

    SDL_Window* window = SDL_CreateWindow("Flower_bullet", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VISIBLE_WIDTH, VISIBLE_HEIGHT, 0);
    if (!window)
    {
        printf("Failed to open %d x %d window: %s\n", VISIBLE_WIDTH, VISIBLE_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 150, 170, 255, 255);//148, 190, 255, 255);
        printf("--- Renderer created ----\n");
    }
    SDL_ShowCursor(SDL_DISABLE);

    TTF_Font* font;
    TTF_Init();
    font = TTF_OpenFont("assets/waltographUI.ttf", 25); //loading font
    if (!font)
        printf("TTF_Init: %s\n", TTF_GetError());

    LinkedList blowingBul;
    LinkedList bullets;
    LinkedList playerBullets;
    LinkedList shapes;
    LinkedList bonuses;

    //EnemyList enemies;

    LinkedList enemies;

    GameObject* player = nullptr;
    GameObject* enemy = nullptr;
    GameObject* bullet = nullptr;
    FontTextures* scoreText = nullptr;
    FontTextures* timeText = nullptr;
    bool isRunning = true;
    bool isMoving = false;
    int levels = LEVELS;
    int shootingPattern = 0;
    int score = 0;
    int counter = 0;
    int shootingDelay = 1000;
    double exploisionTime = 0;
    double buletStartTime = 0;
    int hitBoxPlayer = 0;
    int hitBoxEnemy = 0;
    double invicibilityFrame = 0;
    double shapeTime = 0;
    double bonusTime = 0;
    bool newTexture = false;
    double lastHitTime = 0;
    double invincibilityTime = 0;
    double shapeTimePeriod = 0;
    double timeSinceLastShot = 0.0;
    const double SHOOT_INTERVAL = 0.2;
    double lastBonusSpawnTime = 0.0;
    double lastBonusDeletionTime = 0.0;
    double startTime = SDL_GetTicks() * 0.001; 

    menu(font, renderer, window, player, enemies, bullets, playerBullets,
        blowingBul, bonuses, shapes);

    PlayerHealthBar* health_bar = new PlayerHealthBar("assets/health_bar.png", renderer, 
        400,2, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT);

    PlayerHealthBar* health_rect = new PlayerHealthBar("assets/health_rect.png", renderer,
        health_bar->xpos, health_bar->ypos, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT);

    timeText = new FontTextures(font, renderer, "TIME: 0:00", VISIBLE_WIDTH - 201, 0, 200, 30);
    scoreText = new FontTextures(font, renderer, "YOUR SCORE: 0" , 0, 0, 200, 30);

    initializeGame(renderer, player, enemy, bullets, enemies, playerBullets, blowingBul, bonuses, shapes,
            isMoving, shootingPattern, score, startTime, levels, hitBoxPlayer, hitBoxEnemy, isRunning);

    while (isRunning)
    {
        double lastTime = 0;
        double shootTime = SDL_GetTicks() * 0.001;
        timeSinceLastShot += shootTime - lastTime;
        lastTime = shootTime;
        SDL_Event event;

        while (SDL_PollEvent(&event))
        { 
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    isRunning = false;
                    break;
                case SDLK_UP:
                    player->dy = -PLAYERS_SPEED;
                    player->verticalFrames = 1;
                    break;
                case SDLK_DOWN:
                    player->dy = PLAYERS_SPEED; 
                    player->verticalFrames = 0;
                    break;
                case SDLK_LEFT:
                    player->dx = -PLAYERS_SPEED;
                    player->verticalFrames = 3;
                    break;
                case SDLK_RIGHT:
                    player->dx = PLAYERS_SPEED;
                    player->verticalFrames = 2;
                    break;
                case SDLK_SPACE:
                    if (timeSinceLastShot >= SHOOT_INTERVAL)
                    {
                        bullet = new GameObject("assets/bullet_pl.png", renderer, 0, 0, PL_BULLET_SPEED,
                            SMALL_BULLET_SIZE, SMALL_BULLET_SIZE, SMALL_BULLET_POWER, player->side, WINDOW_WIDTH, WINDOW_HEIGHT);
                        bullet->xpos = player->xpos + player->width / 2;
                        bullet->ypos = player->ypos + player->height / 4;
                        bullet->dx += PL_BULLET_SPEED;
                        playerBullets.addObject(*bullet);
                        newTexture = false;
                        timeSinceLastShot = 0.0;
                    }
                    break;        
                case SDLK_n:
                    initializeGame(renderer, player, enemy,  bullets, enemies,
                        playerBullets, blowingBul, bonuses, shapes,
                        isMoving, shootingPattern, score, startTime, levels, hitBoxPlayer, hitBoxEnemy, isRunning);
                    health_bar->width = HEALTH_BAR_WIDTH;
                    break;
                }  
            }
            else if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    player->dx = 0;
                    player->dy = 0;
                    player->verticalFrames = 4;
                    break;
                case SDLK_DOWN:
                    player->dx = 0;
                    player->dy = 0;
                    player->verticalFrames = 4;
                    break;
                case SDLK_LEFT:
                    player->dx = 0;
                    player->dy = 0;
                    player->verticalFrames = 4;
                    break;
                case SDLK_RIGHT:
                    player->dx = 0;
                    player->dy = 0;
                    player->verticalFrames = 4;
                    break;
                }
            }
        }

       //updates
        
        double elapsedTime = SDL_GetTicks() * 0.001 - startTime;
        std::string timeTextstr = "TIME: " + std::to_string(elapsedTime);
        timeText->setText(timeTextstr);

        std::string scoreTextstr = "YOUR SCORE: " + std::to_string(score);
        scoreText->setText(scoreTextstr);

       double currentTime = SDL_GetTicks() * 0.001;

        // Spawn bonuses every 5 seconds
        if (currentTime - lastBonusSpawnTime >= 5.0)
        {
            initBonus(renderer, bonuses);
            lastBonusSpawnTime = currentTime;
        }  

        if (!bonuses.isEmpty())
        {
            bonuses.updateAllObjects(isMoving);
            if (player)
            {
                bonuses.BonusCollision(*player, PLAYER_HEALTH, *health_bar);
            }
            if (currentTime - lastBonusDeletionTime >= 12.0)
            {
                bonuses.deleteObjects();
                lastBonusDeletionTime = currentTime;
            }
        }

        if (!enemies.isEmpty())
        {
            enemies.updateAllObjects(isMoving);
            enemies.IsAlive();
            enemies.shapesInRect();

            shootingDelay -= 16; // Decrease the delay based on the frame rate (16 milliseconds)
            if (shootingDelay <= 0)
            {
                // Loop through all enemies to check their patterns and make them shoot
                Node* temp = enemies.getHead();
                while (temp)
                {
                    GameObject& currentEnemy = temp->data;
                    int pattern = currentEnemy.shootPattern;
                    if (pattern == 0)//circle bullets
                    {
                        circleBullets(&currentEnemy, bullets, renderer, 0, 12);
                        shootingDelay = 300; // Reset the delay
                    }
                    else if (pattern == 1)//blowing bullets
                    {
                        circleBullets(&currentEnemy, blowingBul, renderer, 2, 8);
                        buletStartTime = SDL_GetTicks();
                        shootingDelay = 2000; // Reset the delay
                    }
                    else if (pattern == 2)//navigating blowing bullets
                    {
                        if (counter % 3 == 0)
                        {
                            shootingDelay = 2000; // Reset the delay
                            counter++;
                            DirectedBullet(&currentEnemy, blowingBul, renderer, player);
                            buletStartTime = SDL_GetTicks();
                        }
                        else
                        {
                            shootingDelay = 400;
                            counter++;
                            DirectedBullet(&currentEnemy, bullets, renderer, player);
                        }
                    }

                    temp = temp->next;
                }
            }
        }

        double exploision = SDL_GetTicks() - buletStartTime;

        if (player)
        {
            player->updateAnimation();

            if (player->health <= 0)
            {
                delete player;
                player = nullptr;
            }

            if (player)
            {
                if (player->xpos < 0)
                {
                    player->xpos = 0; // Set player's X position to the left boundary
                }
                else if (player->xpos > WINDOW_WIDTH - player->width)
                {
                    player->xpos = WINDOW_WIDTH - player->width; // Set player's X position to the right boundary
                }

                if (player->ypos < 0)
                {
                    player->ypos = 0; // Set player's Y position to the top boundary
                }
                else if (player->ypos > WINDOW_HEIGHT - (player->height))
                {
                    player->ypos = WINDOW_HEIGHT - (player->height); // Set player's Y position to the bottom boundary
                }
            
                // Update the camera position based on the player's position
                camera.x = player->xpos - VISIBLE_WIDTH / 2;
                camera.y = player->ypos - VISIBLE_HEIGHT / 2;
    
                // Ensure the camera stays within the bounds of the stage
                if (camera.x < 0) {
                    camera.x = 0;
                }
                if (camera.x > WINDOW_WIDTH - VISIBLE_WIDTH) {
                    camera.x = WINDOW_WIDTH - VISIBLE_WIDTH;
                }
                if (camera.y < 0) {
                    camera.y = 0;
                }
                if (camera.y > WINDOW_HEIGHT - VISIBLE_HEIGHT) {
                    camera.y = WINDOW_HEIGHT - VISIBLE_HEIGHT;
                }
            }
        }
       
        if (!shapes.isEmpty())
        {
            shapes.updateAllObjects(isMoving);
            shapes.shapesInRect();

            double isDangerous = SDL_GetTicks() * 0.001 - shapeTime;
            if (isDangerous > 4.0 && newTexture ==false)
            {
                shapes.updateShapesTex("assets/Shape_after.png");
                newTexture = true;
                shapeTimePeriod = SDL_GetTicks() * 0.001;
            }
            if (newTexture == true)
            {
                if (player)
                {
                    invincibilityTime = SDL_GetTicks() * 0.001 - lastHitTime;
                    if (invincibilityTime > 1)
                    {
                        shapes.hasShapeCollision(*player, score, hitBoxEnemy, lastHitTime);
                    }
                }
            } 
            double currentTime = SDL_GetTicks() * 0.001;
            if ((currentTime - shapeTimePeriod) > 10 && newTexture==true)
            {
                shapes.deleteObjects();
            }
        }

        if (!blowingBul.isEmpty()) //bullet explodes
        {
            blowingBul.updateAllObjects(isMoving);
            blowingBul.bulletInRect();
            if (player)
            {
                invincibilityTime = SDL_GetTicks() * 0.001 - lastHitTime;
                if (invincibilityTime > 1)
                {
                    blowingBul.hasCollisionPlayer(*player, score, hitBoxEnemy, lastHitTime, *health_bar);
                }
            }
            if (exploision >= 1800)
            {
                Node* temp = blowingBul.getHead();
                while (temp)
                {
                    int smallBulletsOnly = 1;
                    circleBullets(&temp->data, bullets, renderer, 1, 20);
                    temp = temp->next;
                }
                blowingBul.deleteObjects();
            }
        }

        if (!bullets.isEmpty())
        {
            bullets.updateAllObjects(isMoving);
            // Check if the bullet reached the end of the window
            bullets.bulletInRect();
            if (player)
            {
                invincibilityTime = SDL_GetTicks() * 0.001 - lastHitTime;
                if (invincibilityTime > 1)
                {
                    bullets.hasCollisionPlayer(*player, score, hitBoxEnemy, lastHitTime, *health_bar);
                }
            }
        }

        if (!playerBullets.isEmpty())
        {
            playerBullets.updateAllObjects(isMoving);
            // Check if the bullet reached the end of the window
            playerBullets.bulletInRect();
            // Check collision only when enemy exists
            if (!enemies.isEmpty())
            {
                Node* enemiesHead = enemies.getHead();
                while (enemiesHead != nullptr)
                {
                    playerBullets.hasCollisionEnemies(enemiesHead->data, score, hitBoxPlayer);
                    enemiesHead = enemiesHead->next;
                }


                /*playerBullets.hasCollisionEnemies(enemiesHead->data, score, hitBoxPlayer);
                if (levels == 1)
                {
                    if (enemiesHead->next != NULL)
                    {
                        playerBullets.hasCollisionEnemies(enemiesHead->next->data, score, hitBoxPlayer);
                    }
                }*/
            }
        }
        health_bar->update();
        health_rect->update();
        
        //renderings
        SDL_RenderClear(renderer);

        if (!bonuses.isEmpty())
        {
            bonuses.renderObjects(renderer, camera.x, camera.y, isMoving);
        }
        if (!enemies.isEmpty())
        {
            enemies.renderObjects(renderer, camera.x, camera.y, isMoving);
        }
        if (!shapes.isEmpty())
        {
            shapes.renderObjects(renderer, camera.x, camera.y, isMoving);
        }
        if (player)
        {
            player->renderAnimation(camera.x, camera.y);
        } 
        if (!bullets.isEmpty())
        {
            bullets.renderObjects(renderer, camera.x, camera.y, isMoving);
        }
        if (!playerBullets.isEmpty())
        {
            playerBullets.renderObjects(renderer, camera.x, camera.y, isMoving);
        }
        if (!blowingBul.isEmpty())
        {
            blowingBul.renderObjects(renderer, camera.x, camera.y, isMoving);
        }
        scoreText->render();
        timeText->render();
        health_bar->render();
        health_rect->render();
        
        SDL_RenderPresent(renderer);
        // Cap the frame rate
        capFrameRate(&then, &remainder);
        //if player is dead - game is over
        if (!player)
        {
            gameOvermenu(font, renderer, window, score, player, enemies, bullets, playerBullets,
                blowingBul, bonuses, shapes);
            menu(font, renderer, window, player, enemies, bullets, playerBullets,
                blowingBul, bonuses, shapes);
            initializeGame(renderer, player, enemy, bullets, enemies,
                playerBullets, blowingBul, bonuses, shapes,
                isMoving, shootingPattern, score, startTime, levels, hitBoxPlayer, hitBoxEnemy, isRunning);
            health_bar->width = HEALTH_BAR_WIDTH;
        }
        //level switch
        if (enemies.isEmpty())
        {
            levels--;
            shootingPattern++;
            if (levels ==2)
            {
                initEnemies(renderer, enemies, shootingPattern, isMoving);
            }
            else if (levels == 1)
            {
                isMoving = true;
                initEnemies(renderer, enemies, shootingPattern, isMoving);
                isMoving = false;
            }
            else if (levels == 0)
            {
                initShapes(renderer, shapes);
                shapeTime = SDL_GetTicks() * 0.001;
            }
        }
        if(levels<0)
        {
            if (shapes.isEmpty())
            {
                if (bullets.isEmpty())
                {
                    isRunning=false;
                    gameOvermenu(font, renderer, window, score, player, enemies, bullets, playerBullets,
                        blowingBul, bonuses, shapes);

                    menu(font, renderer, window, player, enemies, bullets, playerBullets,
                        blowingBul, bonuses, shapes);
                    initializeGame(renderer, player, enemy, bullets, enemies,
                        playerBullets, blowingBul, bonuses, shapes,
                        isMoving, shootingPattern, score, startTime, levels, hitBoxPlayer, hitBoxEnemy, isRunning);
                    health_bar->width = HEALTH_BAR_WIDTH;
                }
            }
        }
    }

    // Clean up
    cleanupGame(player, enemies, bullets, playerBullets, blowingBul, bonuses, shapes);
    if (timeText)
    {
        delete timeText;
        timeText = nullptr;
    }
    if (scoreText)
    {
        delete scoreText;
        scoreText = nullptr;
    }
  
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);  
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    printf("Game cleaned\n");
    return 0;
}


void capFrameRate(long* then, float* remainder)
{
    long wait, frameTime;

    wait = 16 + *remainder;

    *remainder -= (int)*remainder;

    frameTime = SDL_GetTicks() - *then;

    wait -= frameTime;

    if (wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    *remainder += 0.667;

    *then = SDL_GetTicks();
}
