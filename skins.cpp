#include "skins.h"

void Skins::loadTexture(const std::string& path) {
    texture = &ResourceManager::getInstance().getTexture(path);
    sprite.setTexture(*texture);
}

void Skins::addAnimation(const std::string& name,
                         int frameWidth, int frameHeight,
                         int row, int frameCount,
                         float frameDuration, bool loop) {
    Animation anim;
    anim.name = name;
    anim.loop = loop;
    anim.frames.reserve(static_cast<std::size_t>(frameCount));

    for (int col = 0; col < frameCount; ++col) {
        AnimationFrame frame;
        frame.rect = sf::IntRect(col * frameWidth,
                                 row * frameHeight,
                                 frameWidth,
                                 frameHeight);
        frame.duration = frameDuration;
        anim.frames.push_back(frame);
    }

    animSystem.addAnimation(name, anim);
}

void Skins::playAnimation(const std::string& name) {
    animSystem.play(name);
}

void Skins::update(float deltaTime) {
    animSystem.update(deltaTime, sprite);
}

sf::Sprite* Skins::getSprite() {
    return &sprite;
}

AnimationSystem& Skins::getAnimSystem() {
    return animSystem;
}
