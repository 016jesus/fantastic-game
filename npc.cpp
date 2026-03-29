#include "npc.h"

NPC::NPC(int vida, int velocidad, const std::string& nombre,
         const std::vector<std::string>& dialogLines)
    : Dociles(vida, velocidad, nombre), dialogLines(dialogLines)
{
}

const std::vector<std::string>& NPC::getDialogLines() const
{
    return dialogLines;
}

void NPC::addDialogLine(const std::string& line)
{
    dialogLines.push_back(line);
}

bool NPC::hasDialog() const
{
    return !dialogLines.empty();
}
