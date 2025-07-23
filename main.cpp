#include "raylib.h"
#include "raymath.h"
#include <string>
#include <cmath>
#include <cstring>
#include <vector>
#include <iostream>
#include "Gun.hpp"
#include "bullets.hpp"
#include "Entity.hpp"

Vector3 getCameraDirection(Camera& camera){
    return Vector3Subtract(camera.target, camera.position);
}

void mainMenu(Camera& camera, const int screenHeight){
    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed(KEY_SPACE))
            return;

        BeginDrawing();
        ClearBackground(RAYWHITE);
            DrawText("Bob's Mansion", 50, (screenHeight - 250) / 2, 92, BLACK);
            DrawText("Press Space to Continue", 50, (screenHeight - 60) / 2, 41, BLACK);
        EndDrawing();
    }
}

int main(void){
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Bob's Mansion");
    InitAudioDevice();

    Camera camera = { 0 };

    mainMenu(camera, screenHeight);

    camera.position = (Vector3){ 0.0f, 1.0f, 5.0f };
    camera.target = (Vector3){ 3.0f, 1.0f, 3.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Gun gun;

    BoundingBox bounds { {-100, -100, -100}, {100, 100, 100} };
    std::vector<Bob> bobs;
    int bobsMurdered = 0;

    for (int i = 0; i < 5; ++i) {
        bobs.push_back(Bob(camera.position));
    }

    Mesh sphereMesh = GenMeshSphere(0.2f, 16, 16);
    Model sphere = LoadModelFromMesh(sphereMesh);

    std::vector<Bullet> bullets = {};
    Sound hit = LoadSound("../assets/hit.wav");

    Vector3 spherePos;
    Vector3 sphereDir;

    DisableCursor();
    SetTargetFPS(30);

    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        moveBullets(bullets, bounds, bobs, hit);
        while (bobs.size() < 5){
            bobs.push_back(Bob(camera.position));
            ++bobsMurdered;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (gun.leftClick() == true){
                bullets.push_back( {camera.position, getCameraDirection(camera) } );
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) gun.rightClick();

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);

                DrawGrid( 20, 1);
                for (auto& cur : bobs) {
                    DrawModel(cur.BobModel, cur.position, 1.0f, WHITE);
                }

                for (auto& cur : bullets) {
                    DrawModel(sphere, cur.position, 1.0f, GREEN);
                }

            EndMode3D();

            DrawText((std::string("Bobs Murdered: ") + std::to_string(bobsMurdered)).c_str(), 10, 10, 40, WHITE);
            gun.draw( {screenWidth/3, screenHeight-750} );

            const float crosshairRadiusOutter = 7;
            const float crosshairRadiusInner = 4;
            DrawCircle(screenWidth/2, screenHeight/2, crosshairRadiusOutter, BLACK);
            DrawCircle(screenWidth/2, screenHeight/2, crosshairRadiusInner, RAYWHITE);

        EndDrawing();
    }
}


