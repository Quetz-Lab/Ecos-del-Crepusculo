#include "SaveManager.h"


void SaveManager::SaveGame(Vector2 playerPos, int health, int level) {
    std::ofstream saveFile("save.txt");
    if (saveFile.is_open()) {
        saveFile << playerPos.x << " " << playerPos.y << " " << health << " " << level << std::endl;
        saveFile.close();
    }
}

Vector2 SaveManager::LoadGame(int& health, int& level) {
    std::ifstream loadFile("save.txt");
    Vector2 playerPos = { 400, 300 };  // Posición por defecto

    if (loadFile.is_open()) {
        loadFile >> playerPos.x >> playerPos.y >> health >> level;
        loadFile.close();
    }
    return playerPos;
}