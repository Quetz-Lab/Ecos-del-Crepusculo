#pragma once
#include "GameObject.h"
using namespace Quetz_LabEDC;
class Projectile :
    public GameObject
{
public:
    Vector2 direction;
    float speed;

    Projectile(Vector2 position, Vector2 direction, float speed);
    void update() override;


};

