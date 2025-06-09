#pragma once
#include "raylib.h"
#include "GameObject.h"
#include <vector>
using namespace Quetz_LabEDC;
class Level
{
private:
	static Level* instance;
	Level() = default;
	Level(const Level&) = delete;
	Level& operator =(const Level&) = delete;
public:
	std::string name;
	Texture2D background;
	Texture collisionMask;
	Image collisionMasking;
	std::vector<GameObject*> levelObjects;

	static Level& getInstance()
	{
		
		if (!instance)
		{
			instance = new Level();
			instance->InitLevel();
		}
		
	}

	void InitLevel()
	{
		background = LoadTexture("World1.png");
		collisionMask = LoadTexture("World1_mak.png");
	
	}

	Level(const std::string& name, const char* backgroundPath);
	void load();
	void update();
	void draw()
	{
		DrawTexture(background, 0, 0, WHITE);
		DrawTexture(collisionMask, 0, 0, WHITE);

	}

	//bool Check

};

