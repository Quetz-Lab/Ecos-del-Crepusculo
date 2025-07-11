#pragma once
#include "raylib.h"
#include "GameObject.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
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
	Texture2D tileset;
	static constexpr int TILE_SIZE = 32;
	static constexpr int MAP_WIDTH = 40;
	static constexpr int MAP_HEIGHT = 25;

	int tileMap[MAP_WIDTH][MAP_HEIGHT];
	int decorMap[MAP_HEIGHT][MAP_WIDTH];


	void loadTileset(const char* path)
	{
		tileset = LoadTexture(path);
		SetTextureFilter(tileset, TEXTURE_FILTER_POINT); // Para que se vea nítido
	}

	Texture2D background;
	Texture collisionMask;
	Image collisionMaskImg;
	unsigned char* imgdata;
	std::vector<GameObject*> levelObjects;

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
		//background = LoadTexture("World1.jpg");
		collisionMaskImg = LoadImage("World1_mask.png");
		
		ImageFormat(&collisionMaskImg, PIXELFORMAT_UNCOMPRESSED_GRAYSCALE);
		imgdata = (unsigned char*)collisionMaskImg.data;

	}
	void loadMapFromFile(const char* filename)
	{
		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cerr << "No se pudo abrir el archivo del mapa: " << filename << std::endl;
			return;
		}


		std::string line;
		int y = 0;

		while (std::getline(file, line) && y < MAP_HEIGHT) {
			std::istringstream ss(line);
			int value;
			int x = 0;

			while (ss >> value && x < MAP_WIDTH) {
				tileMap[y][x] = value;
				x++;
			}

			y++;
		}

		file.close();
		std::cout << "Mapa cargado correctamente desde " << filename << std::endl;

	}

	void loadDecorationFromFile(const char* filename)
	{
		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cerr << "No se pudo abrir el archivo de decoracion: " << filename << std::endl;
			return;
		}
		std::string line;
		int y = 0;
		while (std::getline(file, line) && y < MAP_HEIGHT) {
			std::istringstream ss(line);
			int value;
			int x = 0;
			while (ss >> value && x < MAP_WIDTH) {
				decorMap[y][x] = value;
				x++;
			}
			y++;
		}
		file.close();
		std::cout << "Decoracion cargada correctamente desde " << filename << std::endl;
	}
	
	Level(const std::string& name, const char* backgroundPath);
	void load();
	void update();
	void draw()
	{
		for (int y = 0; y < MAP_HEIGHT; y++) {
			for (int x = 0; x < MAP_WIDTH; x++) {
				int tileIndex = tileMap[y][x];

				Rectangle source = { tileIndex * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
				Vector2 position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };

				DrawTextureRec(tileset, source, position, WHITE);
			}
		}

		for (int y = 0; y < MAP_HEIGHT; y++) {
			for (int x = 0; x < MAP_WIDTH; x++) {
				int decorIndex = decorMap[y][x];
				if (decorIndex != 0) { // Si hay decoracion
					Rectangle source = { decorIndex * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
					Vector2 position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
					DrawTextureRec(tileset, source, position, WHITE);
				}
			}
		}

	
	}

	bool CheckCollision(Vector2 point)
	{

		int index = (point.y * collisionMaskImg.width + point.x);
		
		std::cout << "valor en pixel (" << point.x << "," << point.y << "): " << (int)imgdata[index] << std::endl;
		return (imgdata[index] == 0);
		return false;
	}

};

