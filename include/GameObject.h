#pragma once
#include "raylib.h"
namespace Quetz_LabEDC
{


    class GameObject
    {
    public:
        Vector2 position;  // Posición en el mundo
        float width, height;  // Tamaño del objeto
        Texture2D texture;  // Textura opcional

        GameObject(float x, float y, float w, float h, Texture2D tex); // Constructor
        virtual void Update();  // Método para actualizar el objeto (puede ser sobreescrito)
        virtual void Draw();  // Método para dibujar el objeto


    };
}
