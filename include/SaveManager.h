#pragma once
#include <fstream>
#include "raylib.h"
#include <string>
#include "json.hpp"
using json = nlohmann::json;


class SaveManager
{
public:
	/*static void SaveGame(Vector2 playerPos, int playerHeath, int level);
	static Vector2 LoadGame(int& playerHeath, int& level);*/
	static void SaveGame(int slot, Vector2 pos, int health, int level, int era);
	static Vector2 LoadGame(int slot, int& health, int& level, int& era);
};

