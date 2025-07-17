#include "raylib.h"
#include "raymath.h"
#include <string>
#include <cmath>
#include <cstring>

void NormalizeAngle(float& angle) {
    if (angle > PI)
        angle = angle-2*PI;
    if (angle < -PI)
        angle = angle+2*PI;
}

void getBobsRotation(float& BobAngle, const Camera camera, const Vector3& BobPosition){

    const float tolerance = DEG2RAD*2;
    const float degreesPerFrame = 1;
    const float turnRate = DEG2RAD*degreesPerFrame;

    auto AngleBetween = atan2( (camera.position.x - BobPosition.x), (camera.position.z - BobPosition.z));
    AngleBetween -= PI/4;

    NormalizeAngle(AngleBetween);

    float diff = std::abs(AngleBetween-BobAngle);

    if (AngleBetween > (BobAngle + tolerance) && diff < PI || AngleBetween < (BobAngle + tolerance) && diff > PI)
        BobAngle += turnRate;
    else if (AngleBetween > (BobAngle - tolerance) && diff > PI || AngleBetween < (BobAngle - tolerance) && diff < PI)
        BobAngle -= turnRate;

    NormalizeAngle(BobAngle);
}

int main(void){
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Bob's Mansion");

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 1.0f, 5.0f };
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Model BobModel = LoadModel("../BobModel.glb"); 
    float BobAngle = -PI/4;
    Vector3 BobPosition = { 3.0f, 0.5f, 3.0f};

    DisableCursor();

    SetTargetFPS(30);

    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            getBobsRotation(BobAngle, camera, BobPosition);
            BobModel.transform = MatrixRotateXYZ( { 0, BobAngle, 0} );

            BeginMode3D(camera);
                DrawModel(BobModel, BobPosition, 1.0f, WHITE);
                DrawPlane( {0, 0, 0}, {20, 20}, GRAY);
            EndMode3D();

        EndDrawing();
    }
}