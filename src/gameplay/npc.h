#pragma once
#ifndef NPC_H
#define NPC_H
#include "entities/base/dociles.h"
#include <vector>
#include <string>

class NPC : public Dociles {
public:
    NPC(int vida, int velocidad, const std::string& nombre,
        const std::vector<std::string>& dialogLines);
    ~NPC() = default;

    const std::vector<std::string>& getDialogLines() const;
    void addDialogLine(const std::string& line);

    // Retorna true si el NPC tiene algo que decir
    bool hasDialog() const;

private:
    std::vector<std::string> dialogLines;
};
#endif
