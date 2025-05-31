#pragma once
#include "raylib.h"
class View
{
public:
	Rectangle rect;
	bool visible = true;

	//ctor por defecto
	View() : rect({ 0, 0, 100, 100 }), visible(true) {}
	//ctor con parametros
	View(int x, int y, int w, int h) : visible(true)
	{
		rect = { (float)x, (float)y, (float)w, (float)h };
	}

	virtual void update() = 0; //metodo virtual puro, debe ser implementado por las clases derivadas
	virtual void draw() = 0; //metodo virtual puro, debe ser implementado por las clases derivadas

protected:
};

