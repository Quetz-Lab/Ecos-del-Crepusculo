#include "UISystem.h"

UISystem* UISystem::instance = nullptr; // Inicializar el puntero estático a nullptr

Label* UISystem::createLabel(const char* text, int x, int y, int fontSize)
{
	Label* label = new Label();
	label->text = text;
	label->rect.x = x;
	label->rect.y = y;
	label->fontSize = fontSize;
	views.push_back(label);
	return label;
}

void UISystem::draw()
{
	for (View* view : views)
	{
		if (view->visible)
			view->draw();
	}
}

void UISystem::update()
{
	for (View* view : views)
	{
		if (view->visible)
			view->update();
	}
}