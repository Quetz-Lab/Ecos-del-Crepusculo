#pragma once
#include "GameObject.h"
namespace Quetz_LabEDC
{
    class Player :
        public GameObject
    {
    public:
        float speed;

        Player(float x, float y, float w, float h, Texture2D tex, float spd);
        void Update() override; // Movimiento del jugador



    };
}

