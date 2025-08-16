/*
    AssetManager.cpp
*/

#include "AssetManager.hpp"

AssetManager::AssetManager() {
    Image image = LoadImage("../assets/cubicmap.png");
    mapImage = LoadTextureFromImage(image);
    Mesh mesh = GenMeshCubicmap(image, (Vector3){ 1.0f, 1.0f, 1.0f });
    defaultRoom = LoadModelFromMesh(mesh);
    Texture2D texture = LoadTexture("../assets/cubicmap_atlas.png");
    defaultRoom.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    UnloadImage(image);

    hitSound = LoadSound("../assets/hit.wav");
    SetSoundVolume(hitSound, 0.2f);
    pewSounds[0] = LoadSound("../assets/Pew1.wav");
    pewSounds[1] = LoadSound("../assets/Pew2.wav");
    pewSounds[2] = LoadSound("../assets/Pew3.wav");

    Mesh sphereMesh = GenMeshSphere(0.05f, 16, 16);
    bulletModel = LoadModelFromMesh(sphereMesh);

    bobModel = LoadModel("../assets/BobModel.glb");

    keyTexture = LoadTexture("../assets/Key.png");
}

AssetManager::~AssetManager() {
    UnloadModel(defaultRoom);
    UnloadTexture(mapImage);

    UnloadSound(hitSound);
    UnloadSound(pewSounds[0]);
    UnloadSound(pewSounds[1]);
    UnloadSound(pewSounds[2]);

    UnloadModel(bulletModel);

    UnloadModel(bobModel);

    UnloadTexture(keyTexture);
}
