#pragma once
#include "raylib.h"
#include "GameObject.h"
#include <vector>
using namespace Quetz_LabEDC;
class Level
{
public:
	std::string name;
	Texture2D background;
	std::vector<GameObject*> levelObjects;
	
	Level(const std::string& name, const char* backgroundPath);
	void load();
	void update();
	void draw();

};

