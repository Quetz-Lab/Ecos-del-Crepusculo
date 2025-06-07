#include "ImageView.h"

ImageView::ImageView(const char* filePath, int x, int y, int w, int h) : View (x, y, w, h) 
{
	texture = LoadTexture(filePath);
}

void ImageView::draw()
{
	if (visible)
	{
		DrawTexture(texture, rect.x, rect.y, WHITE);
	}
}
