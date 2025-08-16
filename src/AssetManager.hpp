/*
    AssetManager.hpp
*/

#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include "raylib.h"
#include <string>
#include <unordered_map>

class AssetManager {
public:

    AssetManager();
    ~AssetManager();

    // Model bulletModel;
    // Texture2D gunTexture;
    // Texture2D gunFiringTexture;
    // Texture2D meleeTexture;
    // Sound PewSounds[3];
    // Sound hitSound;

    // Texture2D keyTexture;

    Texture2D mapImage;
    Model defaultRoom;

    // Model bobModel;
};

#endif
