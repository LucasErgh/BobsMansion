/*
    Game.hpp
*/

#ifndef GAME_HPP
#define GAME_HPP

#include "Scene.hpp"
#include "AssetManager.hpp"
#include "Gun.hpp"
#include "bullets.hpp"
#include "Entity.hpp"
#include "Item.hpp"
#include "raylib.h"
#include "raymath.h"
#include <string>
#include <cmath>
#include <cstring>
#include <vector>
#include <iostream>

struct entity{
    Vector3 pos;
    Vector3 velocity;
    Quaternion rotation;
};

enum WindowState{
    MainMenu,
    InGame
};

class Game{
public:
    Game();

    void InitializeWindow();
    void run();

    bool cursorLookCollision();
    void processInGame();

    void drawGame();
    void drawScene();
    void drawHud();

    Vector3 getCameraDirection(Camera& camera);

public:
    float screenWidth = 800;
    float screenHeight = 450;

    Camera camera = { 0 };

    AssetManager assets;
    Gun gun;
    Scene scene = {};

    bool colhit = false;
    bool inMainMenu = true;

    int bobsMurdered = 0;
    bool promptInteract;
    float reachDistance = 2.0f;

    Vector3 spherePos = { 16.0f, 0.5f, 8.0f };
    float sphereRadius = 0.25f;
    Vector3 sphereDir;

    Item key;

    WindowState winState = MainMenu;
};

#endif
