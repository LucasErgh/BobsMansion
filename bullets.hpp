
#ifndef BULLETS_HPP
#define BULLETS_HPP

#include "raylib.h"
#include "raymath.h"
#include <vector>

struct Bullet{
    Vector3 position;
    Vector3 velocity;
};

void moveBullets(std::vector<Bullet>& bullets, BoundingBox& bounds, std::vector<BoundingBox>& hitboxes, Sound& hit){
    const float velocityScalar = 2.0f;
    auto cur = bullets.begin();
    while(cur != bullets.end()){
        if (cur->position.x > bounds.max.x || cur->position.y > bounds.max.y || cur->position.z > bounds.max.z
            || cur->position.x < bounds.min.x || cur->position.y < bounds.min.y || cur->position.z < bounds.min.z)
        {
            bullets.erase(cur);
        }
        else {
            Ray ray;
            Vector3 startPos = cur->position;
            ray.position = cur->position;
            cur->position = Vector3Add(cur->position, Vector3Scale(cur->velocity, velocityScalar));
            ray.direction = Vector3Subtract(cur->position, ray.position);
            bool collision = false;
            for (auto& box : hitboxes){
                auto col = GetRayCollisionBox(ray, box);
                if (col.hit && Vector3Distance(col.point, startPos) < Vector3Distance(cur->position, startPos)){
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

#endif
