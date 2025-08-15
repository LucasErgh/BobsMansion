
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <cmath>

class Bob {
public:
    static Model* BobModel;
    BoundingBox spawnRange = { {-30, 0, -30}, {10, 10, -40} };
    Vector3 position;

    ~Bob(){
    }

    static Model* GetBobModel() {
        if (BobModel == nullptr) {
            BobModel = new Model();
            (*BobModel) = LoadModel("../assets/BobModel.glb");
        }
        return BobModel;
    }

    Bob(Vector3 cameraPos) : position(cameraPos){
        // position.x = rand() % (int)(spawnRange.max.x - spawnRange.min.x) + (int)spawnRange.min.x;
        // position.y = rand() % (int)(spawnRange.max.y - spawnRange.min.y) + (int)spawnRange.min.y;
        // position.z = rand() % (int)(spawnRange.max.z - spawnRange.min.z) + (int)spawnRange.min.z;

        // auto angle = atan2(position.x - cameraPos.x, position.z - cameraPos.z);
        // BobModel.transform = MatrixRotateXYZ( {0, angle + PI - PI/4, 0});
    }

    BoundingBox getTranslatedBoundingBox() {
        BoundingBox hitbox = GetModelBoundingBox(*BobModel);
        hitbox.min = Vector3Add(hitbox.min, position);
        hitbox.max = Vector3Add(hitbox.max, position);
        return hitbox;
    }

    void DrawBobModel() {
        DrawModel(*GetBobModel(), position, 1.0f, WHITE);
    }
};

Model* Bob::BobModel = nullptr;

#endif
