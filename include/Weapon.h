#pragma once
#include "GameObject.h"
#include "IAttacker.h"

#include <iostream>

namespace  Quetz_LabEDC

{
	class Player; // Forward declaration
	class Weapon :public GameObject, public IAttacker
	{
	public:

		//quien carga el arma
		Player* owner;
		Vector2 offset; // Desplazamiento del arma respecto al jugador

		//constructor
		Weapon(Vector2 pos, std::string _name, Texture tex) :
			GameObject(pos, _name, tex)
		{
			offset = { 30.0f, 10.0f };
		}


		void Fire() override
		{
			// Implementación del ataque
			std::cout << "Weapon fired!" << std::endl;
		}

		void update() override
		{
			//if (owner != nullptr)
			//{
			//	// Actualizar la posición del arma según la del jugador
			//	position = owner->position + offset;
			//}
			//std::cout << "Weapon update at position: " << position.x << ", " << position.y << std::endl;
		}

	};
}

