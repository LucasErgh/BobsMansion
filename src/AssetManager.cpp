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

}

AssetManager::~AssetManager() {
    UnloadModel(defaultRoom);
    UnloadTexture(mapImage);
}
