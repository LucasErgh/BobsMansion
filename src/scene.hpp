
/*
    hpp
*/

#ifndef HPP
#define HPP

#include "Entity.hpp"
#include "Item.hpp"
#include "bullets.hpp"
#include <string>
#include <vector>

struct Scene{
    std::string name;
    std::vector<Bob> bobs;
    std::vector<Item> items;
    std::vector<Bullet> bullets;
    Vector3 os;
    BoundingBox bounds;

    bool updateBullets(const AssetManager& assets) {
        const float velocityScalar = 0.5f;
        auto cur = bullets.begin();
        bool collision = false;
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
                collision = false;
                auto curBox = bobs.begin();
                auto col = GetRayCollisionBox(ray, curBox->getTranslatedBoundingBox(assets));
                if (col.hit) {
                    while (curBox != bobs.end()){
                        if (col.hit && Vector3Distance(col.point, startPos) < Vector3Distance(cur->position, startPos)) {
                            bobs.erase(curBox);
                            PlaySound(assets.hitSound);
                            bullets.erase(cur);
                            collision = true;
                            break;
                        }
                        else {
                            curBox++;
                        }
                    }
                }
                else {
                    col = GetRayCollisionMesh(ray, *assets.defaultRoom.meshes, assets.defaultRoom.transform);
                    if (col.hit && Vector3Distance(col.point, startPos) < Vector3Distance(cur->position, startPos)){
                        collision = true;
                        bullets.erase(cur);
                        PlaySound(assets.hitSound);
                        break;
                    }
                }

                if (!collision)
                    ++cur;
            }
        }
        return collision;
    }
};

#endif
