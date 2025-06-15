#pragma once
#include "raylib.h"
#include "GameObject.h"
#include <vector>
#include <iostream>
using namespace Quetz_LabEDC;
class Level
{
private:
	static Level* instance;
	Level() = default;
	Level(const Level&) = delete;
	Level& operator =(const Level&) = delete;
public:
	//std::string name;
	Texture2D background;
	Texture collisionMask;
	Image collisionMaskImg;
	unsigned char* imgdata;
	//std::vector<GameObject*> levelObjects;

	static Level& getInstance()
	{
		
		if (!instance)
		{
			instance = new Level();
			instance->InitLevel();
		}
		return *instance;
		
	}

	void InitLevel()
	{
		background = LoadTexture("World1.jpg");
		collisionMaskImg = LoadImage("World1_mask.png");
		/*if (collisionMask == {0})
		{
			std::cout << "No se pudo cargar la imagen de colision" << std::endl;
		}*/
		ImageFormat(&collisionMaskImg, PIXELFORMAT_UNCOMPRESSED_GRAYSCALE);
		imgdata = (unsigned char*)collisionMaskImg.data;
	
	}

	Level(const std::string& name, const char* backgroundPath);
	void load();
	void update();
	void draw()
	{
		DrawTexture(background, 0, 0, WHITE);
		//DrawTexture(collisionMask, 0, 0, WHITE);

	}

	bool CheckCollision(Vector2 point)
	{

		int index = (point.y * collisionMaskImg.width + point.x);
		
		std::cout << "valor en pixel (" << point.x << "," << point.y << "): " << (int)imgdata[index] << std::endl;
		return (imgdata[index] != 0);
		//return false;
	}

};

