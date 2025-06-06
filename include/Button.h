#pragma once
#include "View.h"
#include <functional>
#include <string>
class Button :
    public View
{
public:
    std::string text;
    int fontSize;
    Color color;
    std::function<void()> onClick; // Callback para el clic

    Button(const std::string& text, int x, int y, int w, int h, Color color, std::function<void()> callback);
    void update() override;
    void draw() override;


};

