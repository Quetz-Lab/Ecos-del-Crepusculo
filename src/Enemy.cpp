#include "Enemy.h"


Enemy::Enemy(Vector2 position, const char* name, Player* player)
    : GameObject(position, name, LoadTexture("enemy.png")), target(player) {
}

void Enemy::update() {
    if (target) {
        Vector2 direction = { target->position.x - position.x, target->position.y - position.y };
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
        }

        position.x += direction.x * 2.0f; // Velocidad
        position.y += direction.y * 2.0f;
    }
}