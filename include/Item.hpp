
#ifndef ITEM_HPP
#define ITEM_HPP

#include "raylib.h"

class Item {
private:
    static Texture2D GetBillBoardImage(){
        static bool loaded = false;
        static Texture2D texture;
        if (loaded == false)
            texture = LoadTexture("../assets/Key.png");
        return texture;
    }

    Vector3 itemPosition;

public:

    Item(Vector3 Pos) : itemPosition(Pos) { }

    void renderItem(Camera& camera){
        DrawBillboard(camera, GetBillBoardImage(), itemPosition, 2.0f, WHITE);
    }
};

#endif
