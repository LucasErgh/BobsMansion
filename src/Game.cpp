/*
    Game.cpp
*/

#include "Game.hpp"

Game::Game() : key( {16.0f, 0.5f, 8.0f} ){

}

void Game::InitializeWindow(){
    assets.loadAssets();

    // mainMenu(camera, screenHeight);

    camera.position = (Vector3){ 0.0f, 0.5f, 5.0f };
    camera.target = (Vector3){ 3.0f, 1.0f, 3.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    scene.bounds = { {-100, -100, -100}, {100, 100, 100} };
    scene.bobs.push_back(Bob({15.0f, -0.5f, 3.0f}));

    assets.loadAssets();
}

void Game::run(){
    while(!WindowShouldClose()){
        const float reachDistance = 2.0f;
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        switch (winState)
        {
        case WindowState::MainMenu:
            if (IsKeyPressed(KEY_SPACE)) {
                winState = WindowState::InGame;
            }
            BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("Bob's Mansion", 50, (screenHeight - 250) / 2, 92, BLACK);
                DrawText("Press Space to Continue", 50, (screenHeight - 60) / 2, 41, BLACK);
            EndDrawing();
            break;
        case WindowState::InGame:
            processInGame();
            break;
        default:
            break;
        }
    }
}

bool Game::cursorLookCollision(){
    RayCollision col = { 0 };
    Ray ray = GetScreenToWorldRay({screenWidth/2, screenHeight/2}, camera);
    col = GetRayCollisionSphere(ray /*{camera.position, getCameraDirection(camera)}*/, spherePos, sphereRadius );

    DrawSphere(spherePos, sphereRadius, BLUE);
    if ( col.hit /*&& (Vector3Distance(key.itemPosition, col.point) <= 0.5f && Vector3Distance(camera.position, key.itemPosition) <= reachDistance)*/) {
        DrawSphere(col.point, 0.125f, ORANGE);
        return true;
    } else return false;
}

void Game::processInGame(){
    scene.updateBullets(assets);

    // if (cursorLookCollision()) {
    //     promptInteract = true;
    // } else {
    //     promptInteract = false;
    // }

    if (IsKeyPressed(KEY_E)){
        if (cursorLookCollision())
            colhit = true;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (gun.leftClick(assets) == true){
            scene.bullets.push_back( {camera.position, getCameraDirection(camera) } );
        }
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        gun.rightClick();
    }
    if (IsWindowResized()){
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        gun.rescaleGun(screenWidth, screenHeight);
    }
    drawGame();
}

void Game::drawGame(){
    BeginDrawing();

        ClearBackground(BLACK);
        BeginMode3D(camera);
            drawScene();
            if (cursorLookCollision()) {
                promptInteract = true;
            } else {
                promptInteract = false;
            }
        EndMode3D();

        drawHud();
    EndDrawing();
}

void Game::drawScene(){
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
    DrawSphere(key.itemPosition, 0.25f, RED);
}

void Game::drawHud(){
    DrawText((std::string("Bobs Murdered: ") + std::to_string(bobsMurdered)).c_str(), 10, 10, 40, WHITE);
    gun.draw( {screenWidth/3, screenHeight-750} );

    const float crosshairRadiusOutter = 7;
    const float crosshairRadiusInner = 4;
    DrawCircle(screenWidth/2, screenHeight/2, crosshairRadiusOutter, BLACK);
    DrawCircle(screenWidth/2, screenHeight/2, crosshairRadiusInner, RAYWHITE);

    const int textSpacing = 60;
    DrawText(std::to_string(camera.position.x).c_str(), 10, 70, 40, WHITE);
    DrawText(std::to_string(camera.position.y).c_str(), 10, 130, 40, WHITE);
    DrawText(std::to_string(camera.position.z).c_str(), 10, 190, 40, WHITE);
    DrawText(std::to_string(gun.scale).c_str(), 10, 250, 40, WHITE);

    // auto col = GetRayCollisionSphere({camera.position, getCameraDirection(camera)}, key.itemPosition, 0.8f );
    // DrawText(std::to_string(Vector3Distance(camera.position, col.point)).c_str(), screenWidth/2, screenHeight/2 - 60, 40, WHITE);
    // DrawText(std::to_string(Vector3Distance(camera.position, key.itemPosition)).c_str(), screenWidth/2, screenHeight/2 - 120, 40, WHITE);

    if (promptInteract)
        DrawText("PICKUP", screenWidth/2, screenHeight/2, 40, WHITE);
    if (colhit)
        DrawText("KEY", 10, 310, 40, WHITE);
}

Vector3 Game::getCameraDirection(Camera& camera){
    return Vector3Subtract(camera.target, camera.position);
}
