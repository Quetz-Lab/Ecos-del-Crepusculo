#pragma once
#include "GameObject.h"

namespace  Quetz_LabEDC

{
    class Weapon :
        public GameObject
    {
        float damage;   // Da�o que inflige el arma
        bool isAttacking; // Estado de ataque

        Weapon(float x, float y, float w, float h, Texture2D tex, float dmg);
        void Attack();   // M�todo para ejecutar el ataque
        void Update() override; // Actualizaci�n de la l�gica del arma


    };
}

