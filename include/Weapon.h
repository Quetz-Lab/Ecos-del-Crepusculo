#pragma once
#include "GameObject.h"
#include "IAttacker.h"

#include <iostream>

namespace  Quetz_LabEDC

{
    class Player;
    class Weapon :
        public GameObject, public IAttacker
    {
        float damage;   // Daño que inflige el arma
        bool isAttacking; // Estado de ataque

        Weapon(float x, float y, float w, float h, Texture2D tex, float dmg);
        void Attack();   // Método para ejecutar el ataque
        void Update() override; // Actualización de la lógica del arma
    public: 
        Player* owner;
        Vector2 offset;

        void Fire() override
        {
            std::cout << "Weapon fired!" << std::endl;

        }

        /*void Update() override
        {

        }*/


    };
}

