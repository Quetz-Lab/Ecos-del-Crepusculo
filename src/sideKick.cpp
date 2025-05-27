#include "Sidekick.h"



    using namespace Quetz_LabEDC;


	void sideKick::update()
	{
		//moverse hacia el jugador hasta cierta distancia


		if (owner != nullptr)
		{
			Vector2 dir = Vector2Subtract(owner->position, position);

			float distance = Vector2Length(dir);

			if (distance > 50)
			{
				dir = Vector2Normalize(dir);
				position = Vector2Add(position, Vector2Scale(dir, speed * GetFrameTime()));
			}
		}

	}

