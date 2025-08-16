
#ifndef GUN_HPP
#define GUN_HPP

#include <raylib.h>

class Gun {
public:
    Texture2D GunTexture;
    Texture2D GunFiring;
    Texture2D Melee;
    int animationFrame = 0;
    bool isShooting = false;
    bool isMeleeing = false;
    const int coolDown = 5;

    static const int numPews = 3;
    int PewIndex = 0;

    Gun() {
        Image GunImage = LoadImage("../assets/Gun.png");
        Image GunFiringImage = LoadImage("../assets/GunFiring.png");
        Image MeleeImage = LoadImage ("../assets/Melee.png");

        GunTexture = LoadTextureFromImage(GunImage);
        GunFiring = LoadTextureFromImage(GunFiringImage);
        Melee = LoadTextureFromImage(MeleeImage);

        UnloadImage(GunImage);
        UnloadImage(GunFiringImage);
        UnloadImage(MeleeImage);
    }

    ~Gun() {
        UnloadTexture(GunTexture);
        UnloadTexture(GunFiring);
        UnloadTexture(Melee);
    }

    void rightClick() {
        if (animationFrame != 0)
            return;
        animationFrame = coolDown;
        isMeleeing = true;
    }

    // returns flag, true - create bullet, false - dont create bullet
    bool leftClick(const AssetManager& assets) {
        if (animationFrame != 0)
            return false;
        animationFrame = coolDown;
        isShooting = true;

        PlaySound(assets.pewSounds[PewIndex++]);
        if (PewIndex >= numPews)
            PewIndex = 0;

        return true;
    }

    void draw(Vector2 drawPos) {
        if (animationFrame != 0)
            --animationFrame;

        if (animationFrame == 0) {
            DrawTexture(GunTexture, drawPos.x, drawPos.y, WHITE);
        } else if (isMeleeing) {
            DrawTexture(Melee, drawPos.x, drawPos.y, WHITE);
        } else if (isShooting) {
            DrawTexture(GunFiring, drawPos.x, drawPos.y, WHITE);
        }
    }
};

#endif
