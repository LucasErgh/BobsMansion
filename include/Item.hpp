
#ifndef ITEM_HPP
#define ITEM_HPP

#include "AssetManager.hpp"
#include "raylib.h"

class Item {
public:
    Vector3 itemPosition;

public:

    Item(Vector3 Pos) : itemPosition(Pos) { }

    void renderItem(Camera& camera, const AssetManager& assets){
        DrawBillboard(camera, assets.keyTexture, itemPosition, 2.0f, WHITE);
    }
};

#endif
