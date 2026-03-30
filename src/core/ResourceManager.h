#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <stdexcept>

class ResourceManager {
public:
    static ResourceManager& getInstance();
    sf::Texture& getTexture(const std::string& path);
    sf::Font& getFont(const std::string& path);
    sf::SoundBuffer& getSoundBuffer(const std::string& path);
    void clear();

private:
    ResourceManager() = default;
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
};
#endif
