/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/
#include <iostream>	// for std::cout, std::endl
#include <string>	// for std::string
#include <vector>	// for std::vector
#include "Player.h"	// utility header for Player class
#include "sideKick.h"	// utility header for sideKick class

#include "raylib.h"
#include "GameObject.h"	// utility header for GameObject class
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

using namespace Quetz_LabEDC;

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	std::cout << "Inicializando sistema de juego..." << std::endl;

	// Create the window and OpenGL context 
	InitWindow(1280, 800, "Ecos del Crepusculo");
		// Unload the logo texture after use

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	Texture2D logo = LoadTexture("Logo.png");
	float alpha = 0.0f;	// Variable to control the alpha transparency of the logo
	float fadeSpeed = 0.5f;	// Speed at which the logo fades in and out
	SetTargetFPS(60);	// Set the target FPS to 60
	std::cout << "Ventana creada, FPS objetivo establecido a 60." << std::endl;
	while (alpha < 1.0f)
	{
		alpha += fadeSpeed;
		BeginDrawing();	// Begin drawing to the window
		ClearBackground(BLACK);	// Clear the background to black
		DrawTexture(logo, (1280 - logo.width) / 2, (800 - logo.height) / 2, Fade(WHITE, alpha));	// Draw the logo texture at the center of the window with fading effect
		EndDrawing();	// End drawing to the window
	
	}
	float timer = 0.0f;	// Variable to keep track of time
	while (timer < 3.0f)
	{
		timer += GetFrameTime();	// Increment timer by the time elapsed since the last frame
		BeginDrawing();	// Begin drawing to the window
		ClearBackground(RAYWHITE);	// Clear the background to white
		DrawTexture(logo, (1280 - logo.width)/2, (800 - logo.height)/2, WHITE);	// Draw the logo texture at position (250, 150)
		EndDrawing();	// End drawing to the window
	}
	UnloadTexture(logo);

	std::vector<GameObject*> gameObjects;

	GameObject* myObj = new GameObject({ 200,200 }, "myObj", LoadTexture("mono.png"));
	myObj->DisplayName = true;
	//push_back agrega un elemento al final del arreglo
	gameObjects.push_back(myObj);  //cast implicito a GameObject*

	//El jugador
	// este constructor ya no existe, ahora el Player establece su textura
	//Player* playerCharacter = new Player({ 0,0 }, "Player1", LoadTexture("boy.png"));
	Player* playerCharacter = new Player({ 100,100 }, "Player1"); 
	playerCharacter->speed = 200.0f;
	//agregando el player pero con un cast explicito estatico
	// estatico quiere decir que se realiza en tiempo de compilacion
	gameObjects.push_back(static_cast<GameObject*>(playerCharacter));

	//prueba de arma
	Weapon* w = new Weapon({ 500, 500 }, "Sword", LoadTexture("sword.png"));
	//playerCharacter->SetWeapon(w); //asignar el arma al jugador
	gameObjects.push_back(w); //agregar el arma al arreglo de gameobjects

	sideKick* sidekick = new sideKick({ 500,0 }, "Foo", LoadTexture("sidekick.png"));
	sidekick->owner = playerCharacter;
	sidekick->DisplayName = true;
	sidekick->speed = 199.0f;
	gameObjects.push_back(sidekick);

	sideKick* sidekick2 = new sideKick({ 800,600 }, "Bar", LoadTexture("karateka.png"));
	sidekick2->owner = playerCharacter;
	sidekick2->DisplayName = true;
	sidekick2->speed = 190.0f;
	gameObjects.push_back(sidekick2);

	//Ejemplo de dynamic_cast
	for (GameObject* obj : gameObjects)
	{
		//esta conversion es en tiempo de ejecucion
		Player* p = dynamic_cast<Player*>(obj);
		if (p)
		{
			p->attack();
		}

		sideKick* sk = dynamic_cast<sideKick*>(obj);
		if (sk != nullptr)
		{
			sk->flee();
		}
	}




	// game loop a 60 fps
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		//aqui van los update
		//actualizar todos los gameobjects
		for (GameObject* obj : gameObjects)
		{
			obj->update();
		}


		// despues de beginDrawing consideraremos los draw
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(SKYBLUE);

		// draw some text using the default font
		DrawText("Ecos del Crepusculo 0.0.1", 10, 10, 24, YELLOW);
		for (GameObject* obj : gameObjects)
		{
			obj->draw();
		}


		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}


	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
