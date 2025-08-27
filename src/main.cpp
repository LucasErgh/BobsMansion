/*
    main.cpp
*/

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
    float screenWidth = 800;
    float screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Bob's Mansion");
    InitAudioDevice();

    Camera camera = { 0 };

    mainMenu(camera, screenHeight);

    camera.position = (Vector3){ 0.0f, 0.5f, 5.0f };
    camera.target = (Vector3){ 3.0f, 1.0f, 3.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    AssetManager assets;

    Gun gun;

    Scene scene = {} ;
    scene.bounds = { {-100, -100, -100}, {100, 100, 100} };

    int bobsMurdered = 0;

    scene.bobs.push_back(Bob({15.0f, -0.5f, 3.0f}));
    // for (int i = 0; i < 5; ++i) {
    //     bobs.push_back(Bob(camera.position));
    // }

    Vector3 spherePos;
    Vector3 sphereDir;

    Item key( {16.0f, 0.5f, 8.0f} );

    DisableCursor();
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        scene.updateBullets(assets);

        static bool colhit = false;
        const float reachDistance = 2.0f;
        if (IsKeyPressed(KEY_E)){
            auto col = GetRayCollisionSphere({camera.position, getCameraDirection(camera)}, key.itemPosition, 2 );
            if (col.hit && (Vector3Distance(camera.position, col.point) < reachDistance || Vector3Distance(camera.position, key.itemPosition) < 0.5f)) {
                colhit = true;
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (gun.leftClick(assets) == true){
                scene.bullets.push_back( {camera.position, getCameraDirection(camera) } );
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            gun.rightClick();
        if (IsWindowResized()){
            gun.rescaleGun(GetScreenWidth(), GetScreenHeight());
        }

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);

                DrawModel(assets.defaultRoom, {0, 0, 0}, 1.0f, WHITE);
                DrawBoundingBox(GetMeshBoundingBox(assets.defaultRoom.meshes[0]), GREEN);

                DrawGrid(20, 1);
                for (auto& cur : scene.bobs) {
                    cur.DrawBobModel(assets);
                }

                for (auto& cur : scene.bullets) {
                    DrawModel(assets.bulletModel, cur.position, 1.0f, GREEN);
                }

                key.renderItem(camera, assets);
                // DrawSphere(key.itemPosition, 2.0f, RED);

            EndMode3D();

            DrawText((std::string("Bobs Murdered: ") + std::to_string(bobsMurdered)).c_str(), 10, 10, 40, WHITE);
            gun.draw( {screenWidth/3, screenHeight-750} );

            // const float crosshairRadiusOutter = 7;
            // const float crosshairRadiusInner = 4;
            // DrawCircle(screenWidth/2, screenHeight/2, crosshairRadiusOutter, BLACK);
            // DrawCircle(screenWidth/2, screenHeight/2, crosshairRadiusInner, RAYWHITE);

            const int textSpacing = 60;
            DrawText(std::to_string(camera.position.x).c_str(), 10, 70, 40, WHITE);
            DrawText(std::to_string(camera.position.y).c_str(), 10, 130, 40, WHITE);
            DrawText(std::to_string(camera.position.z).c_str(), 10, 190, 40, WHITE);
            DrawText(std::to_string(gun.scale).c_str(), 10, 250, 40, WHITE);

            if (colhit)
                DrawText("KEY", 10, 250, 40, WHITE);

        EndDrawing();
    }
}


