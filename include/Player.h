#pragma once
#include "GameObject.h"
#include "IAttacker.h"
#include "sideKick.h"
#include "Weapon.h"
namespace Quetz_LabEDC
{
	enum EAnimDirection
	{
		ANIM_DOWN, //caminar mirando hacia abajo, la pose prederminada
		ANIM_UP,
		ANIM_LEFT,
		ANIM_RIGHT
	};
	struct SAnimData
	{
		int currentFrame;
		int maxFrames;
		//numero de frames que han de pasar para cambiar de frame en el sprite
		int frameCounter;
		int frameSpeed = 10;
		float spriteWidth;
		float spriteHeight;
		EAnimDirection direction;
	};
    class Player :
		public GameObject, public IAttacker
    {
	private:
		//info de su animacion
		SAnimData animData;

		//instrumento que le permite atacar
		IAttacker* weapon;
		//Sidekick* coso;
	public:
		float speed = 10.0f;

		//constructor heredado de GameObject
		Player(Vector2 pos, std::string _name) :
			weapon(nullptr)
			//sidekicks{ nullptr, nullptr, nullptr }
		{
			texture = LoadTexture("boy-r.png");
			animData.spriteHeight = 80;
			animData.spriteWidth = 64;
			animData.frameCounter = 0;
			animData.currentFrame = 0;
			animData.maxFrames = 4;
			animData.frameSpeed = 5;
			name = _name;
			position = pos;
			animData.direction = ANIM_DOWN;
		}


		void update() override;
		//sobrecargar Draw para dibujar el sprite
		void draw() override;

		void attack()
		{
			std::cout << name << "Atacando" << std::endl;
		}

		//hay que sobrecargar esta funcion a fuerzas
		void Fire();

		IAttacker* SetWeapon(IAttacker* newWeapon);
		//void SetSidekick(Sidekick* newsidekick, int index);

    };
}

