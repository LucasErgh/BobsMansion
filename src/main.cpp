/*
    main.cpp
*/

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
    const int screenWidth = 1920;
    const int screenHeight = 1080;

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

    BoundingBox bounds { {-100, -100, -100}, {100, 100, 100} };
    std::vector<Bob> bobs;
    int bobsMurdered = 0;

    bobs.push_back(Bob({15.0f, -0.5f, 3.0f}));
    // for (int i = 0; i < 5; ++i) {
    //     bobs.push_back(Bob(camera.position));
    // }

    std::vector<Bullet> bullets = {};

    Vector3 spherePos;
    Vector3 sphereDir;

    Item key( {30, 3, 30} );

    DisableCursor();
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        const float velocityScalar = 0.5f;
        auto cur = bullets.begin();
        while(cur != bullets.end()){
            if (cur->position.x > bounds.max.x || cur->position.y > bounds.max.y || cur->position.z > bounds.max.z
                || cur->position.x < bounds.min.x || cur->position.y < bounds.min.y || cur->position.z < bounds.min.z)
            {
                bullets.erase(cur);
            }
            else {
                Ray ray;
                Vector3 startPos = cur->position;
                ray.position = cur->position;
                cur->position = Vector3Add(cur->position, Vector3Scale(cur->velocity, velocityScalar));
                ray.direction = Vector3Subtract(cur->position, ray.position);
                bool collision = false;
                auto curBox = bobs.begin();
                auto col = GetRayCollisionBox(ray, curBox->getTranslatedBoundingBox(assets));
                DrawRay({col.point, col.normal}, GREEN);
                if (col.hit) {
                    while (curBox != bobs.end()){
                        if (col.hit && Vector3Distance(col.point, startPos) < Vector3Distance(cur->position, startPos)) {
                            bobs.erase(curBox);
                            PlaySound(assets.hitSound);
                            bullets.erase(cur);
                            collision = true;
                            break;
                        }
                        else {
                            curBox++;
                        }
                    }
                }
                else {
                    col = GetRayCollisionMesh(ray, *assets.defaultRoom.meshes, assets.defaultRoom.transform);
                    if (col.hit && Vector3Distance(col.point, startPos) < Vector3Distance(cur->position, startPos)){
                        collision = true;
                        bullets.erase(cur);
                        PlaySound(assets.hitSound);
                        break;
                    }
                }

                if (!collision)
                    ++cur;
            }
        }

        static bool colhit = false;
        const float reachDistance = 2.0f;
        if (IsKeyPressed(KEY_E)){
            auto col = GetRayCollisionSphere({camera.position, getCameraDirection(camera)}, key.itemPosition, 2 );
            if (col.hit && (Vector3Distance(camera.position, col.point) < reachDistance)){
                colhit = true;
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (gun.leftClick(assets) == true){
                bullets.push_back( {camera.position, getCameraDirection(camera) } );
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            gun.rightClick();

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);

                DrawModel(assets.defaultRoom, {0, 0, 0}, 1.0f, WHITE);
                DrawBoundingBox(GetMeshBoundingBox(assets.defaultRoom.meshes[0]), GREEN);

                DrawGrid(20, 1);
                for (auto& cur : bobs) {
                    cur.DrawBobModel(assets);
                }

                for (auto& cur : bullets) {
                    DrawModel(assets.bulletModel, cur.position, 1.0f, GREEN);
                }

                key.renderItem(camera);
                DrawSphere(key.itemPosition, 2.0f, RED);

            EndMode3D();

            DrawText((std::string("Bobs Murdered: ") + std::to_string(bobsMurdered)).c_str(), 10, 10, 40, WHITE);
            gun.draw( {screenWidth/3, screenHeight-750} );

            // const float crosshairRadiusOutter = 7;
            // const float crosshairRadiusInner = 4;
            // DrawCircle(screenWidth/2, screenHeight/2, crosshairRadiusOutter, BLACK);
            // DrawCircle(screenWidth/2, screenHeight/2, crosshairRadiusInner, RAYWHITE);

            DrawText(std::to_string(camera.position.x).c_str(), 10, 70, 40, WHITE);
            DrawText(std::to_string(camera.position.y).c_str(), 10, 130, 40, WHITE);
            DrawText(std::to_string(camera.position.z).c_str(), 10, 190, 40, WHITE);

            if (colhit)
                DrawText("KEY", 10, 250, 40, WHITE);

        EndDrawing();
    }
}


