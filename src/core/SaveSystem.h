#pragma once
#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H
#include <string>

struct SaveData {
    std::string playerName;
    std::string gender;    // "male" o "female"
    int hp        = 100;
    int money     = 0;
    float posX    = 64.f;
    float posY    = 148.f;
    int section   = 0;
};

class SaveSystem {
public:
    static SaveSystem& getInstance();

    // Guarda la partida en "save.dat"
    bool save(const SaveData& data);

    // Carga la partida desde "save.dat"
    // Retorna false si no existe o está corrupto
    bool load(SaveData& out);

    // Retorna true si existe un archivo de save válido
    bool hasSave() const;

    // Borra el archivo de save
    void deleteSave();

private:
    SaveSystem() = default;
    static constexpr const char* SAVE_FILE = "save.dat";
};
#endif
