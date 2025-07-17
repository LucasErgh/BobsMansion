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

Matrix getBobsRotation(float& BobAngle, const Camera camera, const Vector3& BobPosition){

    float verticalRotation = atan2(BobPosition.y - camera.position.y, sqrt( (camera.position.x - BobPosition.x)*(camera.position.x - BobPosition.x) + (camera.position.y - BobPosition.y)*(camera.position.y - BobPosition.y)));

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

    return MatrixRotateXYZ( { 0, BobAngle, 0});
}

void mainMenu(Camera& camera, const int screenHeight){
    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed(KEY_SPACE))
            return;

        BeginDrawing();
        ClearBackground(RAYWHITE);
            DrawText("Bob's Mansion", 50, (screenHeight - 92 - 41) / 2, 92, BLACK);
            DrawText("Press Space to Continue", 50, screenHeight/2 + 140, 41, BLACK);
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
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Model Martian = LoadModel("../assets/MartianBustV3.glb");
    Model BobModel = LoadModel("../assets/BobModel.glb"); 
    Vector3 MartianPosition = { -25.0f, 7.0f, 0.0f};

    Image GunImage = LoadImage("../assets/Gun.png");
    Image GunFiringImage = LoadImage("../assets/GunFiring.png");
    Image MeleeImage = LoadImage ("../assets/Melee.png");
    Texture2D Gun = LoadTextureFromImage(GunImage);
    Texture2D GunFiring = LoadTextureFromImage(GunFiringImage);
    Texture2D Melee = LoadTextureFromImage(MeleeImage);
    Rectangle GunDrawLocation = { (screenWidth+750)/2, screenHeight-750, screenWidth, screenHeight};
    Rectangle MeleeDrawLocation = { screenWidth/2, screenHeight-750, screenWidth/2+750, screenHeight};

    float BobAngle = -PI/4;
    Vector3 BobPosition = { 3.0f, 0.5f, 3.0f };

    const int numPews = 3;
    int PewIndex = 0;
    Sound Pews[numPews] = { LoadSound("../assets/Pew1.wav"), LoadSound("../assets/Pew2.wav"), LoadSound("../assets/Pew3.wav") };
    int isShooting = 0;
    int isMeleeing = 0;

    DisableCursor();
    SetTargetFPS(30);

    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !isShooting && !isMeleeing) {
            isShooting = 5;
            PlaySound(Pews[PewIndex]);
            ++PewIndex;
            if (PewIndex >= numPews)
                PewIndex = 0;
        } else if (isShooting) {
            --isShooting;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !isShooting && !isMeleeing) {
            isMeleeing = 5;
        } else if (isMeleeing) {
            --isMeleeing;
        }

        BeginDrawing();
            ClearBackground(BLACK);

            BobModel.transform = getBobsRotation(BobAngle, camera, BobPosition);

            BeginMode3D(camera);

                DrawModel(Martian, MartianPosition, 4.0f, WHITE);
                DrawModel(BobModel, BobPosition, 1.0f, WHITE);
                DrawPlane( {0, 0, 0}, {20, 20}, GRAY);

            EndMode3D();

            if (isShooting)
                DrawTexture(GunFiring, screenWidth/3, screenHeight-Gun.height, WHITE);
            else if (isMeleeing)
                DrawTexture(Melee, screenWidth/3, screenHeight-Melee.height, WHITE);
            else
                DrawTexture(Gun, screenWidth/3, screenHeight-Gun.height, WHITE);
            // DrawTexturePro(Gun, {0, 0, 750, 750}, GunDrawLocation, {0, 0}, 0, WHITE);

            const float crosshairRadiusOutter = 7;
            const float crosshairRadiusInner = 4;
            DrawCircle(screenWidth/2, screenHeight/2, crosshairRadiusOutter, BLACK);
            DrawCircle(screenWidth/2, screenHeight/2, crosshairRadiusInner, RAYWHITE);

        EndDrawing();
    }
}