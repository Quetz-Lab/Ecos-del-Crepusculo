#include "GameObject.h"

namespace Quetz_LabEDC
{



    GameObject::GameObject(float x, float y, float w, float h, Texture2D tex) {
        position = { x, y };
        width = w;
        height = h;
        texture = tex;
    }

    void GameObject::Update() {
        // Por defecto, no hace nada. Se sobreescribirá en clases hijas
    }

    void GameObject::Draw() {
        if (texture.id != 0) {
            DrawTexture(texture, position.x, position.y, WHITE);
        }
        else {
            DrawRectangle(position.x, position.y, width, height, GRAY);
        }
    }
}
