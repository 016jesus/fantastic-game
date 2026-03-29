#pragma once
#ifndef SKINS_H
#define SKINS_H
#include <SFML/Graphics.hpp>
#include <string>
#include "AnimationSystem.h"
#include "ResourceManager.h"

class Skins {
public:
    Skins() = default;

    // Loads the texture from path via ResourceManager and binds it to the sprite.
    void loadTexture(const std::string& path);

    // Registers a sprite-sheet animation.
    // frameWidth / frameHeight : pixel size of each frame cell.
    // row          : row in the sprite sheet (0-indexed, used when the sheet
    //                has multiple rows; pass 0 for single-row sheets).
    // frameCount   : number of frames in this animation.
    // frameDuration: seconds each frame is displayed.
    // loop         : whether the animation repeats after the last frame.
    void addAnimation(const std::string& name,
                      int frameWidth, int frameHeight,
                      int row, int frameCount,
                      float frameDuration, bool loop = true);

    void playAnimation(const std::string& name);
    void update(float deltaTime);

    // Returns a pointer to the internal sprite (never null after construction).
    sf::Sprite* getSprite();
    AnimationSystem& getAnimSystem();

private:
    // Non-owning pointer — lifetime managed by ResourceManager.
    sf::Texture* texture = nullptr;
    sf::Sprite sprite;
    AnimationSystem animSystem;
};
#endif
