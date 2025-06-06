#include "Button.h"
#include "raylib.h"

Button::Button(const std::string& text, int x, int y, int w, int h, Color color, std::function<void()> callback)
    : View(x, y, w, h), text(text), fontSize(20), color(color), onClick(callback) {
}

void Button::update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), rect)) {
        if (onClick) onClick();  // Ejecuta la función asignada al botón
    }
}

void Button::draw() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);  // Dibuja el botón
    DrawText(text.c_str(), rect.x + 10, rect.y + 10, fontSize, WHITE);  // Dibuja el texto encima del botón
}