#pragma once
#include "raylib.h"
#include "string"
#include <iostream>
namespace Quetz_LabEDC
{


    class GameObject
    {
	public:
		Vector2 position;
		std::string name;
		//imagen del objeto
		Texture texture;
		bool DisplayName = false;

		//constructor predeterminado
		GameObject() :
			position({ 0,0 }),
			name("GameObject"),
			texture({ 0 }) {
		}


		//constructor
		GameObject(Vector2 pos, std::string _name, Texture tex) :
			position(pos), name(_name), texture(tex) {
		}

		//actualizar posicion
		virtual void update();
		// dibujar o renderizar el objeto
		virtual void draw();

    };
}
