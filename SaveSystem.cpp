#include "SaveSystem.h"
#include <fstream>
#include <sstream>
#include <cstdio>

SaveSystem& SaveSystem::getInstance() {
    static SaveSystem instance;
    return instance;
}

bool SaveSystem::save(const SaveData& data) {
    std::ofstream f(SAVE_FILE);
    if (!f.is_open()) return false;
    f << data.playerName << "\n"
      << data.gender     << "\n"
      << data.hp         << "\n"
      << data.money      << "\n"
      << data.posX       << "\n"
      << data.posY       << "\n"
      << data.section    << "\n";
    return f.good();
}

bool SaveSystem::load(SaveData& out) {
    std::ifstream f(SAVE_FILE);
    if (!f.is_open()) return false;
    if (!(f >> out.playerName >> out.gender >> out.hp
            >> out.money >> out.posX >> out.posY >> out.section))
        return false;
    return true;
}

bool SaveSystem::hasSave() const {
    std::ifstream f(SAVE_FILE);
    return f.good();
}

void SaveSystem::deleteSave() {
    std::remove(SAVE_FILE);
}
