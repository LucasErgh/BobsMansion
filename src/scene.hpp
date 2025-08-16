
/*
    scene.hpp
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Entity.hpp"
#include "Item.hpp"
#include <string>
#include <vector>

struct Scene {
    std::string name;
    std::vector<Bob> bobs;
    std::vector<Item> items;
};

#endif
