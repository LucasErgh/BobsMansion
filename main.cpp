#include "raylib.h"
#include "raymath.h"

int main(void){
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Bob's Mansion");

    Camera camera = { 0 };
    camera.position = (Vector3){ 5.0f, 5.0f, 5.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Model BobModel = LoadModel("../BobModel.glb"); 

    Vector3 BobPosition = { 0.0f, 0.0f, 0.0f};

    DisableCursor();

    SetTargetFPS(30);

    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            BeginMode3D(camera);
                DrawModel(BobModel, BobPosition, 1.0f, WHITE);
                DrawPlane( {-10, 0, -10}, {20, 20}, BLUE);
            EndMode3D();
        EndDrawing();
    }
}