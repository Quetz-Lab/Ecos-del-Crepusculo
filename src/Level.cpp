#include "Level.h"


Level::Level(const std::string& name, const char* backgroundPath) : name(name) {
    background = LoadTexture(backgroundPath);
}

void Level::load() {
    // Aquí podríamos agregar objetos específicos del nivel (enemigos, portales)
}

void Level::update() {
    for (GameObject* obj : levelObjects) {
        obj->update();
    }
}

void Level::draw() {
    DrawTexture(background, 0, 0, WHITE);
    for (GameObject* obj : levelObjects) {
        obj->draw();
    }
}