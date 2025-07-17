#include "raylib.h"
#include "raymath.h"
#include <string>
#include <cmath>
#include <cstring>
#include <vector>
#include <iostream>

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

void moveBullets(std::vector<std::pair<Vector3, Vector3>>& bullets, BoundingBox& bounds, std::vector<BoundingBox>& hitboxes, Sound& hit){
    const float velocityScalar = 2.0f;
    auto cur = bullets.begin();
    while(cur != bullets.end()){
        if (cur->first.x > bounds.max.x || cur->first.y > bounds.max.y || cur->first.z > bounds.max.z
            || cur->first.x < bounds.min.x || cur->first.y < bounds.min.y || cur->first.z < bounds.min.z)
        {
            bullets.erase(cur);
        }
        else {
            Ray ray;
            Vector3 startPos = cur->first;
            ray.position = cur->first;
            cur->first = Vector3Add(cur->first, Vector3Scale(cur->second, velocityScalar));
            ray.direction = Vector3Subtract(cur->first, ray.position);
            bool collision = false;
            for (auto& box : hitboxes){
                auto col = GetRayCollisionBox(ray, box);
                if (col.hit && Vector3Distance(col.point, startPos) < Vector3Distance(cur->first, startPos)){
                    std::cout << "Collision Distance: " << Vector3Distance(col.point, startPos) << ", Bullet Travel: " << Vector3Distance(cur->first, startPos) << '\n';
                    PlaySound(hit);
                    bullets.erase(cur);
                    collision = true;
                }
            }
            if (!collision)
                ++cur;
        }
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

    Image GunImage = LoadImage("../assets/Gun.png");
    Image GunFiringImage = LoadImage("../assets/GunFiring.png");
    Image MeleeImage = LoadImage ("../assets/Melee.png");
    Texture2D Gun = LoadTextureFromImage(GunImage);
    Texture2D GunFiring = LoadTextureFromImage(GunFiringImage);
    Texture2D Melee = LoadTextureFromImage(MeleeImage);
    Rectangle GunDrawLocation = { (screenWidth+750)/2, screenHeight-750, screenWidth, screenHeight};
    Rectangle MeleeDrawLocation = { screenWidth/2, screenHeight-750, screenWidth/2+750, screenHeight};
    BoundingBox bounds { {-100, -100, -100}, {100, 100, 100} };


    const int numPews = 3;
    int PewIndex = 0;
    Sound Pews[numPews] = { LoadSound("../assets/Pew1.wav"), LoadSound("../assets/Pew2.wav"), LoadSound("../assets/Pew3.wav") };
    int isShooting = 0;
    int isMeleeing = 0;

    Mesh cubeMesh = GenMeshCube(1.0f, 2.0f, 2.0f);
    Model cube = LoadModelFromMesh(cubeMesh);

    Mesh sphereMesh = GenMeshSphere(0.4f, 16, 16);
    Model sphere = LoadModelFromMesh(sphereMesh);

    std::vector<std::pair<Vector3, Vector3>> bullets = {};
    Sound hit = LoadSound("../assets/hit.wav");

    std::vector<BoundingBox> hitboxes;
    hitboxes.push_back(GetMeshBoundingBox(cube.meshes[0]));

    Vector3 spherePos;
    Vector3 sphereDir;

    DisableCursor();
    SetTargetFPS(30);

    float dt = 0;

    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        moveBullets(bullets, bounds, hitboxes, hit);

        dt += GetFrameTime();
        cube.transform = MatrixRotateX(dt/10.0f);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !isShooting && !isMeleeing) {
            spherePos = camera.position;
            sphereDir = Vector3Subtract(camera.target, spherePos);
            bullets.push_back(std::make_pair(spherePos, sphereDir));

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
            DrawText(std::to_string(dt).c_str(), 10, 10, 20, WHITE);
            DrawText(("CamTarget: " + std::to_string(camera.target.x) + std::to_string(camera.target.y) + std::to_string(camera.target.z)).c_str(), 10, 20, 20, WHITE);

            BeginMode3D(camera);

                DrawModelWires(cube, {0, 1, 0}, 1.0f, BLUE);
                DrawPlane( {0, 0, 0}, {20, 20}, GRAY);
                for (auto& cur: bullets){
                    DrawModel(sphere, cur.first, 1.0f, GREEN);
                }

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


