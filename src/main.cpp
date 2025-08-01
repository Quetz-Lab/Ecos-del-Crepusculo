#include "SaveManager.h"	// utility header for SaveManager class
#include <iostream>	// for std::cout, std::endl
#include <string>	// for std::string
#include <vector>	// for std::vector
#include "Player.h"	// utility header for Player class
#include "sideKick.h"	// utility header for sideKick class
#include "raylib.h"
#include "GameObject.h"	// utility header for GameObject class
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "UISystem.h"	// utility header for UISystem class
#include "Button.h"	// utility header for Button class
#include "Enemy.h"	// utility header for Enemy class
#include "Weapon.h"	// utility header for Weapon class
#include "Projectile.h"	// utility header for Projectile class
#include "Level.h"
using namespace Quetz_LabEDC;

enum MenuOption
{
	PLAY, SETTINGS, EXIT
};
MenuOption selectedOption = PLAY;
// variable to keep track of the selected menu option

int main()
{

	int health = 100;
	int level = 1;
	int energy = 50;
	UISystem::getInstance().InitHUD(health, level, energy);
	
	int currentEra = 0;  // 0: Prehistoria, 1: Edad Media, 2: Futuro...
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	std::cout << "Inicializando sistema de juego..." << std::endl;

	// Create the window and OpenGL context 
	InitWindow(1280, 800, "Ecos del Crepusculo");
	// Unload the logo texture after use

// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	Level::getInstance();
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
		DrawTexture(logo, (1280 - logo.width) / 2, (800 - logo.height) / 2, WHITE);	// Draw the logo texture at position (250, 150)
		EndDrawing();	// End drawing to the window
	}
	UnloadTexture(logo);

	//std::vector<GameObject*> gameObjects;
	Texture2D hudBar = LoadTexture("HealthBar(Frame).png");
	SetTextureFilter(hudBar, TEXTURE_FILTER_POINT); // Evita desenfoque en escalado
	GameObject* myObj = new GameObject({ 200,200 }, "myObj", LoadTexture("mono.png"));
	myObj->DisplayName = true;
	//push_back agrega un elemento al final del arreglo
	GameObject::gameObjects.push_back(myObj);  //cast implicito a GameObject*

	//El jugador
	// este constructor ya no existe, ahora el Player establece su textura
	//Player* playerCharacter = new Player({ 0,0 }, "Player1", LoadTexture("boy.png"));
	Player* playerCharacter = new Player({ 270,480 }, "Player1");
	playerCharacter->speed = 200.0f;
	//agregando el player pero con un cast explicito estatico
	// estatico quiere decir que se realiza en tiempo de compilacion
	GameObject::gameObjects.push_back(static_cast<GameObject*>(playerCharacter));

	//prueba de arma
	Weapon* w = new Weapon({ 500, 500 }, "Sword", LoadTexture("sword.png"));
	//playerCharacter->SetWeapon(w); //asignar el arma al jugador
	GameObject::gameObjects.push_back(w); //agregar el arma al arreglo de gameobjects

	sideKick* sidekick = new sideKick({ 500,0 }, "Foo", LoadTexture("sidekick.png"));
	sidekick->owner = playerCharacter;
	sidekick->DisplayName = true;
	sidekick->speed = 199.0f;
	GameObject::gameObjects.push_back(sidekick);

	sideKick* sidekick2 = new sideKick({ 800,600 }, "Bar", LoadTexture("karateka.png"));
	sidekick2->owner = playerCharacter;
	sidekick2->DisplayName = true;
	sidekick2->speed = 190.0f;
	GameObject::gameObjects.push_back(sidekick2);

	//Ejemplo de dynamic_cast
	for (GameObject* obj : GameObject::gameObjects)
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
	// inicializar los elementos de UI
	//UISystem::getInstance().test(); // probar el singleton de UI
	//UISystem::Test(); // probar el metodo estatico del singleton de UI

	/*for (int i = 0; i < 20; i++)
	{
		UISystem::getInstance().createLabel(TextFormat("weeeeee %d", i), 100 + i * 10, 100 + i * 30, 12);
	}
	*/
	///UISystem::getInstance().createLabel("Bienbenido a mi juej0", 400, 400, 48);

	while (!WindowShouldClose()) {
		if (alpha < 1.0f) {
			alpha += fadeSpeed;
		}




			// Cambiar selección con flechas
			if (IsKeyPressed(KEY_S)) selectedOption = (MenuOption)((selectedOption + 1) % 3);
			if (IsKeyPressed(KEY_W)) selectedOption = (MenuOption)((selectedOption - 1 + 3) % 3);

			BeginDrawing();
			ClearBackground(DARKGRAY);

			DrawText("ECOS DEL CREPUSCULO", 480, 200, 40, Fade( WHITE, alpha));
			DrawText((selectedOption == PLAY ? "> " : "  " "Jugar"), 550, 300, 30, Fade(WHITE, alpha));
			DrawText((selectedOption == SETTINGS ? "> " : "  " "Configuracion"), 550, 350, 30, Fade(WHITE, alpha));
			DrawText((selectedOption == EXIT ? "> " : "  " "Salir"), 550, 400, 30,Fade (WHITE, alpha));
			DrawText("Ecos del Crepusculo 0.0.1", 10, 10, 24, WHITE);

			EndDrawing();

			if (IsKeyPressed(KEY_ENTER)) {
				if (selectedOption == PLAY) {
					while (alpha > 0.0f) {
						alpha -= fadeSpeed;

						BeginDrawing();
						ClearBackground(DARKGRAY);
						DrawText("ECOS DEL CREPÚSCULO", 480, 200, 40, Fade(WHITE, alpha));
						DrawText("Cargando...", 550, 350, 30, Fade(WHITE, alpha));
						EndDrawing();
					}
					break;  // Inicia el juego
				}
				if (selectedOption == EXIT) {
					CloseWindow();
					return 0;
				}


			}
				
	}

	Level::getInstance().loadTileset("TileSetDeco.png"); // Load the tileset for the level
	Level::getInstance().loadMapFromFile("mapa.txt"); // Load the map from a file
	Level::getInstance().loadDecorationFromFile("decoration.txt"); // Load the decoration from a file


		// game loop a 60 fps
		while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
		{
			

			if (IsKeyPressed(KEY_H)) health -= 10; // Ejemplo de cambio de estado
			if (IsKeyPressed(KEY_E)) energy -= 5;
			if (IsKeyPressed(KEY_L)) level++;
			if (IsKeyPressed(KEY_SPACE)) {
				Vector2 dir = { 1.0f, 0.0f };  // Disparo hacia la derecha
				Projectile* newProjectile = new Projectile(playerCharacter->position, dir, 5.0f);
				GameObject::gameObjects.push_back(newProjectile);
			}
			Button* spawnEnemyButton = new Button("Spawn Enemigo", 50, 500, 200, 50, DARKGRAY, [=]() {
				//sideKick* newsideKck = new sideKick({ rand() % 800, rand() % 600 }, "sideKick", LoadTexture("Algo.png"));
				Enemy* newEnemy = new Enemy({ (float)(rand() % 800), (float)(rand() % 600) }, "Enemy", playerCharacter);
				GameObject::gameObjects.push_back(newEnemy);
				});
			UISystem::getInstance().views.push_back(spawnEnemyButton);
			UISystem::getInstance().UpdateHUD(health, level, energy);
			//aqui van los update
			//actualizar todos los gameobjects
			for (GameObject* obj : GameObject::gameObjects)
			{
				obj->update();
			}
			UISystem::Update(); //actualizar el sistema de UI

			// despues de beginDrawing consideraremos los draw
			BeginDrawing();

			// Setup the back buffer for drawing (clear color and depth buffers)
			ClearBackground(SKYBLUE);
			Level::getInstance().draw();
			//DrawRectangle(10, 10, 100, 100, RED); // Si esto aparece, Raylib está dibujando bien.
			//DrawRectangle(10, 10, 100, 20, RED); // Test visual

			//DrawTexture(hudBar, 10, 10, WHITE); // draw the health bar frame
			Rectangle sourceRec = { 0, 0, hudBar.width, hudBar.height };  // Región completa de la imagen
			Rectangle destRec = { 10, 10, hudBar.width * 4, hudBar.height * 4 }; // Escalado x4
			Vector2 origin = { 0, 0 };  // Punto de origen

			DrawTexturePro(hudBar, sourceRec, destRec, origin, 0.0f, WHITE); // draw the health bar frame with source and destination rectangles

			// draw some text using the default font
			
			for (GameObject* obj : GameObject::gameObjects)
			{
				obj->draw();
			}

			UISystem::Draw();
			// end the frame and get ready for the next one  (display frame, poll input, etc...)
			EndDrawing();
		}
		// destroy the window and cleanup the OpenGL context
		CloseWindow();
		return 0;
};