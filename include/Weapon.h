#pragma once
#include "GameObject.h"

namespace  Quetz_LabEDC

{
    class Weapon :
        public GameObject
    {
        float damage;   // Daño que inflige el arma
        bool isAttacking; // Estado de ataque

        Weapon(float x, float y, float w, float h, Texture2D tex, float dmg);
        void Attack();   // Método para ejecutar el ataque
        void Update() override; // Actualización de la lógica del arma


    };
}

