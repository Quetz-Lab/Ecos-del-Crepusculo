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

void UISystem::InitHUD(int health, int level, int energy) {
	healthLabel = createLabel(("Vida: " + std::to_string(health)).c_str(), 50, 30);
	
	levelLabel = createLabel(("Nivel: " + std::to_string(level)).c_str(), 50, 60);
	energyLabel = createLabel(("Energía: " + std::to_string(energy)).c_str(), 50, 90);
}

void UISystem::UpdateHUD(int health, int level, int energy) 
{
	
	sprintf(buffer, "Health: %i", health);
	healthLabel->text = buffer;
	sprintf(buffer2, "Level: %i", level);
	levelLabel->text = buffer2;
	sprintf(buffer3, "Energy: %i", energy);
	energyLabel->text = buffer3;
	//healthLabel->text = ("Health: " + std::to_string(health)).c_str();
	/*levelLabel->text = ("Level: " + std::to_string(level)).c_str();
	energyLabel->text = ("Energy: " + std::to_string(energy)).c_str();*/
}

