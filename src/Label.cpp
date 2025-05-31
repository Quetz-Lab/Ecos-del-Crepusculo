#include "Label.h"

void Label::update()
{
}

void Label::draw()
{
	DrawText(text, rect.x, rect.y, fontSize, BLACK);
}