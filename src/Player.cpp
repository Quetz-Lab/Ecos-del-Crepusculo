#include "Player.h"
namespace Quetz_LabEDC
{

	Player::Player(float x, float y, float w, float h, Texture2D tex, float spd)
		: GameObject(x, y, w, h, tex), speed(spd) {}

	void Player::Update() 
	{
		if (IsKeyDown(KEY_RIGHT)) position.x += speed;
		if (IsKeyDown(KEY_LEFT)) position.x -= speed;
		if (IsKeyDown(KEY_UP)) position.y -= speed;
		if (IsKeyDown(KEY_DOWN)) position.y += speed;
	}

}
