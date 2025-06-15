#include "SaveManager.h"


void SaveManager::SaveGame(int slot, Vector2 pos, int health, int level, int era) {
    json saveData;
    saveData["position"] = { {"x", pos.x}, {"y", pos.y} };
    saveData["health"] = health;
    saveData["level"] = level;
    saveData["era"] = era;

    std::ofstream file("save_slot" + std::to_string(slot) + ".json");
    if (file.is_open()) {
        file << saveData.dump(4); // Escribe con indentación bonita
        file.close();
    }
}

Vector2 SaveManager::LoadGame(int slot, int& health, int& level, int& era) {
    Vector2 pos = { 400, 300 }; // Posición por defecto
    std::ifstream file("save_slot" + std::to_string(slot) + ".json");
    if (file.is_open()) {
        json saveData;
        file >> saveData;

        pos.x = saveData["position"]["x"];
        pos.y = saveData["position"]["y"];
        health = saveData["health"];
        level = saveData["level"];
        era = saveData["era"];

        file.close();
    }
    return pos;
}
