#pragma once
#include "View.h"
class Image :
    public View, 
{
    Texture2D texture;

    Image(const char* filePath, int x, int y, int w, int h);
    void draw() override;
};

