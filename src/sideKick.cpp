#include "Sidekick.h"

namespace Quetz_LabEDC
{
    using namespace Quetz_LabEDC;


    sideKick::sideKick(float x, float y, float w, float h, Texture2D tex, float speed)
        : GameObject(x, y, w, h, tex), followSpeed(speed) 
    {
        targetPosition = { x, y };
    }

    void sideKick::Follow(Vector2 playerPos) {
        targetPosition = playerPos;

        // Movimiento suave hacia la posición del jugador
        position.x += (targetPosition.x - position.x) * followSpeed;
        position.y += (targetPosition.y - position.y) * followSpeed;
    }

    void sideKick::Update() 
    {
        Follow(targetPosition);
    }

}