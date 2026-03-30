#include "AnimationSystem.h"

void AnimationSystem::addAnimation(const std::string& name, const Animation& anim) {
    animations[name] = anim;
}

void AnimationSystem::play(const std::string& name) {
    if (name == current) {
        return;
    }
    auto it = animations.find(name);
    if (it == animations.end()) {
        return;
    }
    current = name;
    currentFrame = 0;
    elapsed = 0.f;
    finished = false;
}

void AnimationSystem::update(float deltaTime, sf::Sprite& sprite) {
    if (current.empty()) {
        return;
    }
    auto it = animations.find(current);
    if (it == animations.end()) {
        return;
    }
    const Animation& anim = it->second;
    if (anim.frames.empty()) {
        return;
    }

    // Apply current frame rect immediately so the sprite always shows something
    sprite.setTextureRect(anim.frames[currentFrame].rect);

    if (finished) {
        return;
    }

    elapsed += deltaTime;

    while (elapsed >= anim.frames[currentFrame].duration) {
        elapsed -= anim.frames[currentFrame].duration;
        std::size_t nextFrame = currentFrame + 1;

        if (nextFrame >= anim.frames.size()) {
            if (anim.loop) {
                currentFrame = 0;
            } else {
                // Stay on last frame and mark as finished
                finished = true;
                sprite.setTextureRect(anim.frames[currentFrame].rect);
                return;
            }
        } else {
            currentFrame = nextFrame;
        }
        sprite.setTextureRect(anim.frames[currentFrame].rect);
    }
}

const std::string& AnimationSystem::currentAnimation() const {
    return current;
}

bool AnimationSystem::isFinished() const {
    return finished;
}
