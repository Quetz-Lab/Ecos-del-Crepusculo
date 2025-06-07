#pragma once
#include "GameObject.h"
#include "Player.h"
using namespace  Quetz_LabEDC;
class Enemy :
    public GameObject
{
public:
    Player* target; // Referencia al jugador

    Enemy(Vector2 position, const char* name, Player* player);
    void update() override;


};

