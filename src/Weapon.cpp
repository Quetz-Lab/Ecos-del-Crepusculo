#include "Weapon.h"

namespace  Quetz_LabEDC
{

    Weapon::Weapon(float x, float y, float w, float h, Texture2D tex, float dmg)
        : GameObject(x, y, w, h, tex), damage(dmg), isAttacking(false) {
    }

    void Weapon::Attack() {
        isAttacking = true;
    }

    void Weapon::Update() {
        if (isAttacking) {
            // Simulaci�n de animaci�n de ataque
            position.x += 5;
        }
        else {
            position.x -= 5;  // Regresa a la posici�n original
        }

        isAttacking = false;  // Reinicia el ataque despu�s de ejecutarlo
    }

}