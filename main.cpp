#include "raylib.h"
#include "raymath.h"
#include <string>
#include <cmath>
#include <cstring>
#include <vector>
#include <iostream>
#include "Gun.hpp"
#include "bullets.hpp"

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

    Mesh cubeMesh = GenMeshCube(1.0f, 2.0f, 2.0f);
    Model cube = LoadModelFromMesh(cubeMesh);

    Mesh sphereMesh = GenMeshSphere(0.2f, 16, 16);
    Model sphere = LoadModelFromMesh(sphereMesh);

    std::vector<Bullet> bullets = {};
    Sound hit = LoadSound("../assets/hit.wav");

    std::vector<BoundingBox> hitboxes;
    hitboxes.push_back(GetMeshBoundingBox(cube.meshes[0]));

    Vector3 spherePos;
    Vector3 sphereDir;

    DisableCursor();
    SetTargetFPS(30);

    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        moveBullets(bullets, bounds, hitboxes, hit);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (gun.leftClick() == true){
                bullets.push_back( {camera.position, getCameraDirection(camera) } );
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) gun.rightClick();

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);

                DrawModelWires(cube, {0, 0, 0}, 1.0f, BLUE);
                DrawPlane( {0, 0, 0}, {20, 20}, GRAY);
                for (auto& cur: bullets){
                    DrawModel(sphere, cur.position, 1.0f, GREEN);
                }

            EndMode3D();

            gun.draw( {screenWidth/3, screenHeight-750} );

            const float crosshairRadiusOutter = 7;
            const float crosshairRadiusInner = 4;
            DrawCircle(screenWidth/2, screenHeight/2, crosshairRadiusOutter, BLACK);
            DrawCircle(screenWidth/2, screenHeight/2, crosshairRadiusInner, RAYWHITE);

        EndDrawing();
    }
}


