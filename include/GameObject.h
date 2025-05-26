#pragma once
#include "raylib.h"
namespace Quetz_LabEDC
{


    class GameObject
    {
    public:
        Vector2 position;  // Posici�n en el mundo
        float width, height;  // Tama�o del objeto
        Texture2D texture;  // Textura opcional

        GameObject(float x, float y, float w, float h, Texture2D tex); // Constructor
        virtual void Update();  // M�todo para actualizar el objeto (puede ser sobreescrito)
        virtual void Draw();  // M�todo para dibujar el objeto


    };
}
