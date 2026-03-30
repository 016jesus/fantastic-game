#include "core/ResourceManager.h"

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

sf::Texture& ResourceManager::getTexture(const std::string& path) {
    auto it = textures.find(path);
    if (it != textures.end()) {
        return it->second;
    }
    sf::Texture& tex = textures[path];
    if (!tex.loadFromFile(path)) {
        textures.erase(path);
        throw std::runtime_error("ResourceManager: failed to load texture: " + path);
    }
    return textures.at(path);
}

sf::Font& ResourceManager::getFont(const std::string& path) {
    auto it = fonts.find(path);
    if (it != fonts.end()) {
        return it->second;
    }
    sf::Font& font = fonts[path];
    if (!font.loadFromFile(path)) {
        fonts.erase(path);
        throw std::runtime_error("ResourceManager: failed to load font: " + path);
    }
    return fonts.at(path);
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& path) {
    auto it = soundBuffers.find(path);
    if (it != soundBuffers.end()) {
        return it->second;
    }
    sf::SoundBuffer& buf = soundBuffers[path];
    if (!buf.loadFromFile(path)) {
        soundBuffers.erase(path);
        throw std::runtime_error("ResourceManager: failed to load sound buffer: " + path);
    }
    return soundBuffers.at(path);
}

void ResourceManager::clear() {
    textures.clear();
    fonts.clear();
    soundBuffers.clear();
}
