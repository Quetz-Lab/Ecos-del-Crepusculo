#pragma once
#include "GameObject.h"

namespace Quetz_LabEDC
{

    class sideKick :
        public GameObject
    {

        public:
            Vector2 targetPosition;  // Posici�n a la que sigue (puede ser el jugador)
            float followSpeed;

            sideKick(float x, float y, float w, float h, Texture2D tex, float speed);
            void Follow(Vector2 playerPos);  // M�todo para seguir al jugador
            void Update() override;  // Sobreescribimos la actualizaci�n


    };
}
