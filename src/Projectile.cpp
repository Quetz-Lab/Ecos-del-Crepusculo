#include "Projectile.h"

Projectile::Projectile(Vector2 position, Vector2 direction, float speed)
    : GameObject(position, "Projectile", LoadTexture("projectile.png")), direction(direction), speed(speed) {
}

void Projectile::update() {
    position.x += direction.x * speed;
    position.y += direction.y * speed;

    // Si el proyectil sale de la pantalla, eliminarlo
    if (position.x < 0 || position.x > GetScreenWidth() || position.y < 0 || position.y > GetScreenHeight()) {
        GameObject::gameObjects.erase(std::remove(GameObject::gameObjects.begin(), GameObject::gameObjects.end(), this), GameObject::gameObjects.end());
        delete this;
    }
}