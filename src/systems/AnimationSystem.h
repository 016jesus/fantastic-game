#pragma once
#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>

struct AnimationFrame {
    sf::IntRect rect;
    float duration; // seconds
};

struct Animation {
    std::string name;
    std::vector<AnimationFrame> frames;
    bool loop = true;
};

class AnimationSystem {
public:
    void addAnimation(const std::string& name, const Animation& anim);
    void play(const std::string& name);
    void update(float deltaTime, sf::Sprite& sprite);
    const std::string& currentAnimation() const;
    bool isFinished() const;

private:
    std::unordered_map<std::string, Animation> animations;
    std::string current;
    std::size_t currentFrame = 0;
    float elapsed = 0.f;
    bool finished = false;
};
#endif
