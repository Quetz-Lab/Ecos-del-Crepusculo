#pragma once
#include <fstream>
#include "raylib.h"


class SaveManager
{
public:
	static void SaveGame(Vector2 playerPos, int playerHeath, int level);
	static Vector2 LoadGame(int& playerHeath, int& level);
};

