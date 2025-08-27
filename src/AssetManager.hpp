/*
    AssetManager.hpp
*/

#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include "raylib.h"
#include "raymath.h"
#include <string>
#include <unordered_map>

class AssetManager {
public:

    AssetManager();
    ~AssetManager();

    void loadAssets();

    Model bulletModel;
    Texture2D gunTexture;
    Texture2D gunFiringTexture;
    Texture2D meleeTexture;
    Sound pewSounds[3];
    Sound hitSound;

    Texture2D keyTexture;

    Texture2D mapImage;
    Model defaultRoom;

    Model bobModel;
};

#endif
