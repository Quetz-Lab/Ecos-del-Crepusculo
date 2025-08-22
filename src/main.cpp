#include "SaveManager.h"	// utility header for SaveManager class
#include <iostream>	// for std::cout, std::endl
#include <string>	// for std::string
#include <vector>	// for std::vector
#include <exception>	// for std::exception
#include <stdexcept>	// for std::runtime_error, std::logic_error
#include <typeinfo>	// for std::type_info
#include <memory>	// for std::unique_ptr
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
//#include "DebugSystem.h"	// utility header for DebugSystem class

// ==================== SISTEMA DE EXCEPCIONES PERSONALIZADAS ====================
/*
 * SISTEMA DE MANEJO DE EXCEPCIONES INTEGRADO
 * 
 * Este sistema proporciona manejo robusto de excepciones con try-catch-throw para
 * capturar y manejar errores en tiempo de ejecución de manera elegante.
 * 
 * CARACTERÍSTICAS:
 * - Clases de excepción personalizadas para diferentes tipos de errores
 * - Captura automática con información detallada (archivo, línea, función)
 * - Integración con el sistema de ventanas de error crítico
 * - Macros para facilitar el uso y manejo de excepciones
 * 
 * TIPOS DE EXCEPCIONES:
 * - GameException: Clase base para todas las excepciones del juego
 * - CriticalGameException: Errores críticos que requieren atención inmediata
 * - ResourceException: Errores relacionados con carga/acceso de recursos
 * - MemoryException: Errores de asignación y gestión de memoria
 * - PerformanceException: Errores relacionados con rendimiento crítico
 * - GameLogicException: Errores de lógica del juego y validación
 * 
 * MACROS DISPONIBLES:
 * - THROW_*_EXCEPTION: Para lanzar excepciones con información automática
 * - SAFE_EXECUTE: Para ejecutar código con manejo automático de excepciones
 * - HANDLE_*_EXCEPTION: Para manejar excepciones capturadas
 * 
 * TECLAS DE SIMULACIÓN:
 * - X: Excepción crítica del juego
 * - Z: Excepción de recurso no encontrado
 * - V: Excepción de memoria insuficiente
 * - B: Excepción de rendimiento crítico
 * - N: Excepción de lógica del juego
 * - J: Excepción estándar de C++
 * - K: Excepción desconocida
 */

// Clase base para todas las excepciones del juego
class GameException : public std::exception {
protected:
    std::string message;
    std::string file;
    int line;
    std::string function;
    
public:
    GameException(const std::string& msg, const std::string& f = "", int l = 0, const std::string& func = "") 
        : message(msg), file(f), line(l), function(func) {}
    
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
    
    const std::string& getFile() const { return file; }
    int getLine() const { return line; }
    const std::string& getFunction() const { return function; }
    
    virtual std::string getErrorType() const { return "GameException"; }
    virtual std::string getDetailedMessage() const {
        std::string detailed = "[" + getErrorType() + "] " + message;
        if (!file.empty()) {
            detailed += "\nArchivo: " + file + ":" + std::to_string(line);
        }
        if (!function.empty()) {
            detailed += "\nFunción: " + function;
        }
        return detailed;
    }
};

// Excepción para errores críticos del juego
class CriticalGameException : public GameException {
public:
    CriticalGameException(const std::string& msg, const std::string& f = "", int l = 0, const std::string& func = "") 
        : GameException(msg, f, l, func) {}
    
    std::string getErrorType() const override { return "Error Crítico"; }
};

// Excepción para errores de recursos
class ResourceException : public GameException {
private:
    std::string resourcePath;
    
public:
    ResourceException(const std::string& msg, const std::string& path, const std::string& f = "", int l = 0) 
        : GameException(msg, f, l), resourcePath(path) {}
    
    std::string getErrorType() const override { return "Error de Recurso"; }
    const std::string& getResourcePath() const { return resourcePath; }
    
    std::string getDetailedMessage() const override {
        std::string detailed = GameException::getDetailedMessage();
        detailed += "\nRecurso: " + resourcePath;
        return detailed;
    }
};

// Excepción para errores de memoria
class MemoryException : public GameException {
private:
    size_t requestedSize;
    size_t availableSize;
    
public:
    MemoryException(const std::string& msg, size_t requested = 0, size_t available = 0, 
                   const std::string& f = "", int l = 0) 
        : GameException(msg, f, l), requestedSize(requested), availableSize(available) {}
    
    std::string getErrorType() const override { return "Error de Memoria"; }
    size_t getRequestedSize() const { return requestedSize; }
    size_t getAvailableSize() const { return availableSize; }
    
    std::string getDetailedMessage() const override {
        std::string detailed = GameException::getDetailedMessage();
        if (requestedSize > 0) {
            detailed += "\nMemoria solicitada: " + std::to_string(requestedSize) + " bytes";
        }
        if (availableSize > 0) {
            detailed += "\nMemoria disponible: " + std::to_string(availableSize) + " bytes";
        }
        return detailed;
    }
};

// Excepción para errores de rendimiento
class PerformanceException : public GameException {
private:
    float currentFPS;
    float minimumFPS;
    
public:
    PerformanceException(const std::string& msg, float current, float minimum, 
                        const std::string& f = "", int l = 0) 
        : GameException(msg, f, l), currentFPS(current), minimumFPS(minimum) {}
    
    std::string getErrorType() const override { return "Error de Rendimiento"; }
    float getCurrentFPS() const { return currentFPS; }
    float getMinimumFPS() const { return minimumFPS; }
    
    std::string getDetailedMessage() const override {
        std::string detailed = GameException::getDetailedMessage();
        detailed += "\nFPS actual: " + std::to_string(currentFPS);
        detailed += "\nFPS mínimo: " + std::to_string(minimumFPS);
        return detailed;
    }
};

// Excepción para errores de lógica del juego
class GameLogicException : public GameException {
private:
    std::string expectedValue;
    std::string actualValue;
    
public:
    GameLogicException(const std::string& msg, const std::string& expected = "", 
                      const std::string& actual = "", const std::string& f = "", int l = 0) 
        : GameException(msg, f, l), expectedValue(expected), actualValue(actual) {}
    
    std::string getErrorType() const override { return "Error de Lógica"; }
    const std::string& getExpectedValue() const { return expectedValue; }
    const std::string& getActualValue() const { return actualValue; }
    
    std::string getDetailedMessage() const override {
        std::string detailed = GameException::getDetailedMessage();
        if (!expectedValue.empty()) {
            detailed += "\nValor esperado: " + expectedValue;
        }
        if (!actualValue.empty()) {
            detailed += "\nValor actual: " + actualValue;
        }
        return detailed;
    }
};

// Macros para lanzar excepciones con información de ubicación
#define THROW_GAME_EXCEPTION(msg) \
    throw GameException(msg, __FILE__, __LINE__, __FUNCTION__)

#define THROW_CRITICAL_EXCEPTION(msg) \
    throw CriticalGameException(msg, __FILE__, __LINE__, __FUNCTION__)

#define THROW_RESOURCE_EXCEPTION(msg, path) \
    throw ResourceException(msg, path, __FILE__, __LINE__)

#define THROW_MEMORY_EXCEPTION(msg, requested, available) \
    throw MemoryException(msg, requested, available, __FILE__, __LINE__)

#define THROW_PERFORMANCE_EXCEPTION(msg, current, minimum) \
    throw PerformanceException(msg, current, minimum, __FILE__, __LINE__)

#define THROW_LOGIC_EXCEPTION(msg, expected, actual) \
    throw GameLogicException(msg, expected, actual, __FILE__, __LINE__)

// Variables globales para el sistema de error
static bool gameErrorOccurred = false;
static std::string errorMessage = "";
static std::string errorDetails = "";
static std::string errorFile = "";
static int errorLine = 0;
static std::unique_ptr<GameException> lastException = nullptr;

// Implementación temporal del sistema de debug
namespace Quetz_LabEDC {
    class DebugSystem {
    public:
        static DebugSystem& getInstance() {
            static DebugSystem instance;
            return instance;
        }
        
        static void Initialize() {
            // Implementación vacía por ahora
        }
        
        static void Update() {
            // Implementación vacía por ahora
        }
        
        static void Draw() {
            // Implementación vacía por ahora
        }
        
        static void Shutdown() {
            // Implementación vacía por ahora
        }
        
        void initialize() {}
        void update() {}
        void draw() {}
        void shutdown() {}
        
        void logDebug(const std::string& message, const char* file, int line) {
            std::cout << "[DEBUG] " << message << " (" << file << ":" << line << ")" << std::endl;
        }
        
        void logInfo(const std::string& message, const char* file, int line) {
            std::cout << "[INFO] " << message << " (" << file << ":" << line << ")" << std::endl;
        }
        
        void logWarning(const std::string& message, const char* file, int line) {
            std::cout << "[WARNING] " << message << " (" << file << ":" << line << ")" << std::endl;
        }
        
        void logError(const std::string& message, const char* file, int line) {
            std::cout << "[ERROR] " << message << " (" << file << ":" << line << ")" << std::endl;
        }
        
        void triggerCriticalError(const std::string& message, const std::string& details, const char* file, int line) {
            std::cout << "[CRITICAL ERROR] " << message << " (" << file << ":" << line << ")" << std::endl;
            gameErrorOccurred = true;
            errorMessage = message;
            errorDetails = details;
            errorFile = file;
            errorLine = line;
        }
        
        // Función para manejar excepciones del juego
        void handleGameException(const GameException& ex) {
            std::cout << "[EXCEPTION CAUGHT] " << ex.getErrorType() << ": " << ex.what() << std::endl;
            gameErrorOccurred = true;
            errorMessage = ex.getErrorType() + ": " + ex.what();
            errorDetails = ex.getDetailedMessage();
            errorFile = ex.getFile();
            errorLine = ex.getLine();
            
            // Guardar la excepción para referencia
            lastException = std::make_unique<GameException>(ex);
        }
        
        // Función para manejar excepciones estándar
        void handleStandardException(const std::exception& ex, const char* file, int line) {
            std::cout << "[STD EXCEPTION] " << ex.what() << " (" << file << ":" << line << ")" << std::endl;
            gameErrorOccurred = true;
            errorMessage = "Excepción Estándar: " + std::string(ex.what());
            errorDetails = "Se ha producido una excepción estándar de C++.\n\n"
                          "Tipo: " + std::string(typeid(ex).name()) + "\n"
                          "Mensaje: " + std::string(ex.what()) + "\n"
                          "Ubicación: " + std::string(file) + ":" + std::to_string(line) + "\n\n"
                          "Esto puede indicar:\n"
                          "• Error de programación\n"
                          "• Condición inesperada\n"
                          "• Problema de recursos del sistema\n"
                          "• Corrupción de datos";
            errorFile = file;
            errorLine = line;
        }
        
        // Función para manejar excepciones desconocidas
        void handleUnknownException(const char* file, int line) {
            std::cout << "[UNKNOWN EXCEPTION] Excepción desconocida capturada (" << file << ":" << line << ")" << std::endl;
            gameErrorOccurred = true;
            errorMessage = "Excepción Desconocida";
            errorDetails = "Se ha producido una excepción de tipo desconocido.\n\n"
                          "Ubicación: " + std::string(file) + ":" + std::to_string(line) + "\n\n"
                          "Esto puede indicar:\n"
                          "• Excepción personalizada no capturada\n"
                          "• Error de sistema de bajo nivel\n"
                          "• Corrupción de memoria\n"
                          "• Problema crítico del hardware\n\n"
                          "Se recomienda reiniciar el juego inmediatamente.";
            errorFile = file;
            errorLine = line;
        }
    };
}

// Macros para logging (definidas después de la clase)
#define DEBUG_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logDebug(msg, __FILE__, __LINE__)
#define INFO_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logInfo(msg, __FILE__, __LINE__)
#define WARN_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logWarning(msg, __FILE__, __LINE__)
#define ERROR_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logError(msg, __FILE__, __LINE__)
#define CRITICAL_ERROR(msg, details) Quetz_LabEDC::DebugSystem::getInstance().triggerCriticalError(msg, details, __FILE__, __LINE__)

// Macros para manejo de excepciones
#define HANDLE_GAME_EXCEPTION(ex) Quetz_LabEDC::DebugSystem::getInstance().handleGameException(ex)
#define HANDLE_STD_EXCEPTION(ex) Quetz_LabEDC::DebugSystem::getInstance().handleStandardException(ex, __FILE__, __LINE__)
#define HANDLE_UNKNOWN_EXCEPTION() Quetz_LabEDC::DebugSystem::getInstance().handleUnknownException(__FILE__, __LINE__)

// Macro para ejecutar código con manejo automático de excepciones
#define SAFE_EXECUTE(code) \
    try { \
        code; \
    } catch (const GameException& ex) { \
        HANDLE_GAME_EXCEPTION(ex); \
    } catch (const std::exception& ex) { \
        HANDLE_STD_EXCEPTION(ex); \
    } catch (...) { \
        HANDLE_UNKNOWN_EXCEPTION(); \
    }

// Macro para funciones que pueden lanzar excepciones
#define SAFE_FUNCTION_CALL(func, ...) \
    try { \
        return func(__VA_ARGS__); \
    } catch (const GameException& ex) { \
        HANDLE_GAME_EXCEPTION(ex); \
        throw; \
    } catch (const std::exception& ex) { \
        HANDLE_STD_EXCEPTION(ex); \
        throw; \
    } catch (...) { \
        HANDLE_UNKNOWN_EXCEPTION(); \
        throw; \
    }

// Función para dibujar la ventana de error
void DrawErrorWindow() {
    if (!gameErrorOccurred) return;
    
    // Dibujar overlay semi-transparente
    DrawRectangle(0, 0, 1280, 800, Fade(BLACK, 0.8f));
    
    // Dimensiones de la ventana de error
    int windowWidth = 600;
    int windowHeight = 400;
    int windowX = (1280 - windowWidth) / 2;
    int windowY = (800 - windowHeight) / 2;
    
    // Dibujar ventana principal
    DrawRectangle(windowX, windowY, windowWidth, windowHeight, DARKGRAY);
    DrawRectangleLines(windowX, windowY, windowWidth, windowHeight, RED);
    DrawRectangleLines(windowX + 2, windowY + 2, windowWidth - 4, windowHeight - 4, LIGHTGRAY);
    
    // Título de la ventana
    DrawText("¡ERROR CRÍTICO DETECTADO!", windowX + 20, windowY + 20, 24, RED);
    DrawLine(windowX + 20, windowY + 50, windowX + windowWidth - 20, windowY + 50, RED);
    
    // Mensaje de error principal
    DrawText("Mensaje:", windowX + 20, windowY + 70, 16, WHITE);
    DrawText(errorMessage.c_str(), windowX + 20, windowY + 90, 14, YELLOW);
    
    // Detalles del error
    DrawText("Detalles:", windowX + 20, windowY + 130, 16, WHITE);
    DrawText(errorDetails.c_str(), windowX + 20, windowY + 150, 12, LIGHTGRAY);
    
    // Información del archivo
    std::string fileInfo = "Archivo: " + errorFile + " (Línea: " + std::to_string(errorLine) + ")";
    DrawText("Ubicación:", windowX + 20, windowY + 190, 16, WHITE);
    DrawText(fileInfo.c_str(), windowX + 20, windowY + 210, 12, LIGHTGRAY);
    
    // Instrucciones
    DrawText("Acciones disponibles:", windowX + 20, windowY + 250, 16, WHITE);
    DrawText("• Presiona ENTER para continuar el juego", windowX + 30, windowY + 275, 12, GREEN);
    DrawText("• Presiona ESC para cerrar el juego", windowX + 30, windowY + 295, 12, ORANGE);
    DrawText("• Presiona R para reiniciar el nivel", windowX + 30, windowY + 315, 12, BLUE);
    
    // Botones
    DrawRectangle(windowX + 50, windowY + 350, 120, 30, GREEN);
    DrawText("CONTINUAR", windowX + 70, windowY + 360, 12, WHITE);
    
    DrawRectangle(windowX + 200, windowY + 350, 120, 30, ORANGE);
    DrawText("REINICIAR", windowX + 220, windowY + 360, 12, WHITE);
    
    DrawRectangle(windowX + 350, windowY + 350, 120, 30, RED);
    DrawText("SALIR", windowX + 390, windowY + 360, 12, WHITE);
}

// Función para manejar input de la ventana de error
void HandleErrorWindowInput() {
    if (!gameErrorOccurred) return;
    
    if (IsKeyPressed(KEY_ENTER)) {
        gameErrorOccurred = false;
        INFO_LOG("Usuario eligió continuar después del error");
    }
    
    if (IsKeyPressed(KEY_R)) {
        gameErrorOccurred = false;
        INFO_LOG("Usuario eligió reiniciar después del error");
        // Aquí podrías agregar lógica de reinicio
    }
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        INFO_LOG("Usuario eligió salir después del error");
        CloseWindow();
    }
}

// La clase DebugSystem ya está definida arriba

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
	
	// Initialize Debug System
	DebugSystem::Initialize();
	INFO_LOG("Sistema de juego inicializado");
	INFO_LOG("Ventana creada: 1280x800");

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
	// Crear GameObject con manejo seguro de excepciones
	GameObject* myObj = nullptr;
	SAFE_EXECUTE({
		Texture2D objTexture = LoadTexture("mono.png");
		if (objTexture.id == 0) {
			THROW_RESOURCE_EXCEPTION("No se pudo cargar la textura del objeto", "mono.png");
		}
		myObj = new GameObject({ 200,200 }, "myObj", objTexture);
		if (!myObj) {
			THROW_MEMORY_EXCEPTION("No se pudo crear el GameObject", sizeof(GameObject), 0);
		}
		myObj->DisplayName = true;
		//push_back agrega un elemento al final del arreglo
		GameObject::gameObjects.push_back(myObj);  //cast implicito a GameObject*
	});

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




			// Cambiar selecci�n con flechas
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
						DrawText("ECOS DEL CREP�SCULO", 480, 200, 40, Fade(WHITE, alpha));
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
	
	INFO_LOG("Nivel cargado correctamente");
	INFO_LOG("Iniciando game loop principal");


		// game loop a 60 fps
		static float frameTimeAccumulator = 0.0f;
		static int frameCount = 0;
		static float lastFPSCheck = 0.0f;
		
		while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
		{
			// Envolver todo el loop del juego con manejo de excepciones
			try {
				// Manejar input de la ventana de error primero
				HandleErrorWindowInput();
			
			// Si hay un error crítico, pausar el juego
			if (gameErrorOccurred) {
				// Solo dibujar la ventana de error, no actualizar el juego
				BeginDrawing();
				ClearBackground(SKYBLUE);
				Level::getInstance().draw();
				
				// Dibujar el juego en el estado actual (pausado)
				for (GameObject* obj : GameObject::gameObjects) {
					obj->draw();
				}
				UISystem::Draw();
				
				// Dibujar la ventana de error encima
				DrawErrorWindow();
				
				EndDrawing();
				continue; // Saltar el resto del loop hasta que se resuelva el error
			}
			
			// Update Debug System
			DebugSystem::Update();
			
			// ERROR SIMULADO 9: Monitoreo de rendimiento
			float currentTime = GetTime();
			float frameTime = GetFrameTime();
			frameTimeAccumulator += frameTime;
			frameCount++;
			
			// Verificar FPS cada segundo
			if (currentTime - lastFPSCheck >= 1.0f) {
				float avgFPS = frameCount / (currentTime - lastFPSCheck);
				
				if (avgFPS < 30.0f) {
					ERROR_LOG("¡RENDIMIENTO CRÍTICO! FPS promedio: " + std::to_string(avgFPS));
					ERROR_LOG("El juego está funcionando por debajo del mínimo aceptable");
				} else if (avgFPS < 45.0f) {
					WARN_LOG("Rendimiento bajo detectado. FPS promedio: " + std::to_string(avgFPS));
				}
				
				// Verificar frame time individual
				if (frameTime > 0.033f) { // Más de 33ms = menos de 30 FPS
					WARN_LOG("Frame lento detectado: " + std::to_string(frameTime * 1000.0f) + "ms");
				}
				
				lastFPSCheck = currentTime;
				frameCount = 0;
			}
			
			// ERROR SIMULADO 10: Tecla para simular crash (presiona C)
			if (IsKeyPressed(KEY_C)) {
				CRITICAL_ERROR("Simulación de acceso a puntero nulo", 
							 "Stack trace simulado:\n"
							 "  -> main.cpp:line_" + std::to_string(__LINE__) + "\n"
							 "  -> GameObject::update()\n"
							 "  -> Player::processInput()\n\n"
							 "En un juego real, esto causaría un crash inmediato.\n"
							 "El sistema de debug ha capturado el error exitosamente.");
			}
			
			// ERROR SIMULADO 11: Tecla para simular memory leak (presiona M)
			if (IsKeyPressed(KEY_M)) {
				CRITICAL_ERROR("Fuga de memoria simulada detectada", 
							 "Se ha simulado la creación de 10 objetos sin liberación de memoria.\n\n"
							 "Objetos creados sin gestión:\n"
							 "• 10x GameObject sin delete\n"
							 "• 10x Texture sin UnloadTexture\n"
							 "• Memoria total perdida: ~2.5MB\n\n"
							 "En un juego real esto causaría:\n"
							 "• Consumo creciente de RAM\n"
							 "• Eventual agotamiento de memoria\n"
							 "• Crash por Out of Memory\n"
							 "• Degradación progresiva del rendimiento\n\n"
							 "Herramientas recomendadas para detección:\n"
							 "• Valgrind (Linux)\n"
							 "• Application Verifier (Windows)\n"
							 "• AddressSanitizer (GCC/Clang)");
				// Simular la creación de objetos con leak
				for (int i = 0; i < 10; i++) {
					GameObject* leakedObj = new GameObject({0, 0}, "LeakedObject_" + std::to_string(i), LoadTexture("mono.png"));
					// Intencionalmente NO agregamos al vector para simular leak
				}
			}

			// ==================== SIMULACIONES DE EXCEPCIONES ====================
			
			// EXCEPCIÓN SIMULADA 1: Tecla para simular excepción crítica (presiona X)
			if (IsKeyPressed(KEY_X)) {
				try {
					THROW_CRITICAL_EXCEPTION("Simulación de excepción crítica del juego");
				} catch (const CriticalGameException& ex) {
					HANDLE_GAME_EXCEPTION(ex);
				}
			}
			
			// EXCEPCIÓN SIMULADA 2: Tecla para simular excepción de recurso (presiona Z)
			if (IsKeyPressed(KEY_Z)) {
				try {
					THROW_RESOURCE_EXCEPTION("Recurso no encontrado o corrupto", "resources/missing_texture.png");
				} catch (const ResourceException& ex) {
					HANDLE_GAME_EXCEPTION(ex);
				}
			}
			
			// EXCEPCIÓN SIMULADA 3: Tecla para simular excepción de memoria (presiona V)
			if (IsKeyPressed(KEY_V)) {
				try {
					THROW_MEMORY_EXCEPTION("Fallo en asignación de memoria", 1024*1024*100, 1024*1024*50);
				} catch (const MemoryException& ex) {
					HANDLE_GAME_EXCEPTION(ex);
				}
			}
			
			// EXCEPCIÓN SIMULADA 4: Tecla para simular excepción de rendimiento (presiona B)
			if (IsKeyPressed(KEY_B)) {
				try {
					float currentFPS = GetFPS();
					THROW_PERFORMANCE_EXCEPTION("Rendimiento crítico detectado", currentFPS, 30.0f);
				} catch (const PerformanceException& ex) {
					HANDLE_GAME_EXCEPTION(ex);
				}
			}
			
			// EXCEPCIÓN SIMULADA 5: Tecla para simular excepción de lógica (presiona N)
			if (IsKeyPressed(KEY_N)) {
				try {
					THROW_LOGIC_EXCEPTION("Valor fuera del rango esperado", "1-100", std::to_string(level));
				} catch (const GameLogicException& ex) {
					HANDLE_GAME_EXCEPTION(ex);
				}
			}
			
			// EXCEPCIÓN SIMULADA 6: Tecla para simular excepción estándar (presiona J)
			if (IsKeyPressed(KEY_J)) {
				try {
					throw std::runtime_error("Simulación de std::runtime_error");
				} catch (const std::exception& ex) {
					HANDLE_STD_EXCEPTION(ex);
				}
			}
			
			// EXCEPCIÓN SIMULADA 7: Tecla para simular excepción desconocida (presiona K)
			if (IsKeyPressed(KEY_K)) {
				try {
					throw "Excepción de tipo const char*";
				} catch (...) {
					HANDLE_UNKNOWN_EXCEPTION();
				}
			}

			if (IsKeyPressed(KEY_H)) {
				health -= 10;
				DEBUG_LOG("Salud reducida a: " + std::to_string(health));
				
				// ERROR SIMULADO 1: Salud negativa
				if (health < 0) {
					CRITICAL_ERROR("Salud del jugador es negativa: " + std::to_string(health), 
								 "Esto podría causar comportamiento indefinido en el sistema de combate.\n"
								 "El juego ha sido pausado para prevenir corrupción de datos.\n"
								 "La salud será corregida automáticamente al continuar.");
					health = 0; // Corrección automática
				}
			}
			if (IsKeyPressed(KEY_E)) {
				energy -= 5;
				DEBUG_LOG("Energía reducida a: " + std::to_string(energy));
				
				// ERROR SIMULADO 2: División por cero potencial
				if (energy <= 0) {
					CRITICAL_ERROR("Energía agotada - Riesgo de división por cero", 
								 "La energía del jugador ha llegado a cero o valores negativos.\n"
								 "Esto podría causar división por cero en:\n"
								 "  • Cálculos de velocidad de movimiento\n"
								 "  • Sistema de regeneración de maná\n"
								 "  • Algoritmos de física del juego\n\n"
								 "La energía será establecida a 1 para prevenir crashes.");
					energy = 1; // Valor mínimo para evitar división por cero
				}
			}
			if (IsKeyPressed(KEY_L)) {
				level++;
				INFO_LOG("Nivel incrementado a: " + std::to_string(level));
				
				// ERROR SIMULADO 3: Overflow de nivel
				if (level > 100) {
					CRITICAL_ERROR("Nivel excede el máximo permitido: " + std::to_string(level), 
								 "Esto podría causar overflow en cálculos de experiencia.\n"
								 "Los valores de experiencia podrían volverse negativos.\n"
								 "El sistema de progresión podría corromperse.\n\n"
								 "El nivel será limitado al máximo seguro al continuar.");
					level = 100;
				}
			}
			if (IsKeyPressed(KEY_SPACE)) {
				// ERROR SIMULADO 4: Verificar límite de proyectiles
				int projectileCount = 0;
				for (GameObject* obj : GameObject::gameObjects) {
					if (dynamic_cast<Projectile*>(obj)) {
						projectileCount++;
					}
				}
				
				if (projectileCount >= 50) {
					CRITICAL_ERROR("Límite de proyectiles excedido: " + std::to_string(projectileCount), 
								 "Se han creado demasiados proyectiles simultáneamente.\n"
								 "Esto puede causar:\n"
								 "  • Caída severa de FPS\n"
								 "  • Consumo excesivo de memoria\n"
								 "  • Posible crash por agotamiento de recursos\n"
								 "  • Lag en detección de colisiones\n\n"
								 "El disparo ha sido bloqueado para proteger el rendimiento.");
				} else {
					Vector2 dir = { 1.0f, 0.0f };  // Disparo hacia la derecha
					
					// ERROR SIMULADO 5: Verificar posición válida del jugador
					if (playerCharacter->position.x < 0 || playerCharacter->position.x > 1280 ||
						playerCharacter->position.y < 0 || playerCharacter->position.y > 800) {
						CRITICAL_ERROR("Jugador fuera de límites del mundo", 
									 "El jugador está en una posición inválida:\n"
									 "Posición actual: (" + std::to_string(playerCharacter->position.x) + 
									 ", " + std::to_string(playerCharacter->position.y) + ")\n"
									 "Límites válidos: (0-1280, 0-800)\n\n"
									 "Esto puede causar:\n"
									 "  • Proyectiles apareciendo fuera de pantalla\n"
									 "  • Errores en detección de colisiones\n"
									 "  • Comportamiento impredecible del jugador\n\n"
									 "La posición será corregida automáticamente.");
						// Corregir posición del jugador
						playerCharacter->position.x = std::max(0.0f, std::min(1280.0f, playerCharacter->position.x));
						playerCharacter->position.y = std::max(0.0f, std::min(800.0f, playerCharacter->position.y));
					}
					
					Projectile* newProjectile = new Projectile(playerCharacter->position, dir, 5.0f);
					GameObject::gameObjects.push_back(newProjectile);
					DEBUG_LOG("Proyectil creado en posición: " + std::to_string(playerCharacter->position.x) + "," + std::to_string(playerCharacter->position.y));
				}
			}
			Button* spawnEnemyButton = new Button("Spawn Enemigo", 50, 500, 200, 50, DARKGRAY, [=]() {
				// ERROR SIMULADO 6: Verificar límite de enemigos
				int enemyCount = 0;
				for (GameObject* obj : GameObject::gameObjects) {
					if (dynamic_cast<Enemy*>(obj)) {
						enemyCount++;
					}
				}
				
				if (enemyCount >= 20) {
					CRITICAL_ERROR("Límite de enemigos excedido: " + std::to_string(enemyCount), 
								 "Se han creado demasiados enemigos simultáneamente.\n"
								 "Esto puede causar problemas graves:\n"
								 "  • Caída crítica de rendimiento\n"
								 "  • Sobrecarga del sistema de IA\n"
								 "  • Lag en pathfinding y colisiones\n"
								 "  • Posible freeze del juego\n\n"
								 "El spawn de enemigos ha sido bloqueado para proteger el sistema.");
					return;
				}
				
				// ERROR SIMULADO 7: Verificar memoria disponible (simulado)
				if (GameObject::gameObjects.size() > 100) {
					CRITICAL_ERROR("Fuga de memoria detectada: " + std::to_string(GameObject::gameObjects.size()) + " objetos", 
								 "El número de GameObjects ha excedido el límite seguro.\n"
								 "Objetos actuales: " + std::to_string(GameObject::gameObjects.size()) + "\n"
								 "Límite recomendado: 100\n\n"
								 "Esto indica una posible fuga de memoria que puede causar:\n"
								 "  • Consumo excesivo de RAM\n"
								 "  • Degradación progresiva del rendimiento\n"
								 "  • Eventual crash por agotamiento de memoria\n\n"
								 "Se recomienda revisar la gestión de objetos del juego.");
				}
				
				float x = (float)(rand() % 800);
				float y = (float)(rand() % 600);
				
				// ERROR SIMULADO 8: Verificar que el enemigo no aparezca encima del jugador
				float distanceToPlayer = sqrt(pow(x - playerCharacter->position.x, 2) + pow(y - playerCharacter->position.y, 2));
				if (distanceToPlayer < 50.0f) {
					CRITICAL_ERROR("Enemigo generado demasiado cerca del jugador", 
								 "Un enemigo fue generado a una distancia peligrosa del jugador.\n"
								 "Distancia actual: " + std::to_string(distanceToPlayer) + " píxeles\n"
								 "Distancia mínima segura: 50 píxeles\n\n"
								 "Esto puede causar:\n"
								 "  • Daño instantáneo no deseado al jugador\n"
								 "  • Experiencia de juego injusta\n"
								 "  • Muerte súbita sin oportunidad de reacción\n\n"
								 "El enemigo será reposicionado automáticamente.");
					// Reposicionar enemigo lejos del jugador
					x = playerCharacter->position.x + (rand() % 2 == 0 ? 100 : -100);
					y = playerCharacter->position.y + (rand() % 2 == 0 ? 100 : -100);
					x = std::max(0.0f, std::min(800.0f, x));
					y = std::max(0.0f, std::min(600.0f, y));
				}
				
				Enemy* newEnemy = new Enemy({ x, y }, "Enemy", playerCharacter);
				GameObject::gameObjects.push_back(newEnemy);
				INFO_LOG("Enemigo creado en posición: " + std::to_string(x) + "," + std::to_string(y));
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
			//DrawRectangle(10, 10, 100, 100, RED); // Si esto aparece, Raylib est� dibujando bien.
			//DrawRectangle(10, 10, 100, 20, RED); // Test visual

			//DrawTexture(hudBar, 10, 10, WHITE); // draw the health bar frame
			Rectangle sourceRec = { 0, 0, hudBar.width, hudBar.height };  // Regi�n completa de la imagen
			Rectangle destRec = { 10, 10, hudBar.width * 4, hudBar.height * 4 }; // Escalado x4
			Vector2 origin = { 0, 0 };  // Punto de origen

			DrawTexturePro(hudBar, sourceRec, destRec, origin, 0.0f, WHITE); // draw the health bar frame with source and destination rectangles

			// draw some text using the default font
			
			for (GameObject* obj : GameObject::gameObjects)
			{
				obj->draw();
			}

			UISystem::Draw();
			
			// Draw Debug System (should be last to render on top)
			DebugSystem::Draw();
			
			// Dibujar ventana de error si es necesario (debe ser lo último)
			DrawErrorWindow();
			
			// end the frame and get ready for the next one  (display frame, poll input, etc...)
			EndDrawing();
			
		} catch (const CriticalGameException& ex) {
			// Manejar excepciones críticas del juego
			HANDLE_GAME_EXCEPTION(ex);
			ERROR_LOG("Excepción crítica capturada: " + std::string(ex.what()));
		} catch (const ResourceException& ex) {
			// Manejar excepciones de recursos
			HANDLE_GAME_EXCEPTION(ex);
			ERROR_LOG("Error de recurso: " + ex.getResourcePath());
		} catch (const MemoryException& ex) {
			// Manejar excepciones de memoria
			HANDLE_GAME_EXCEPTION(ex);
			ERROR_LOG("Error de memoria: " + std::to_string(ex.getRequestedSize()) + " bytes solicitados");
		} catch (const PerformanceException& ex) {
			// Manejar excepciones de rendimiento
			HANDLE_GAME_EXCEPTION(ex);
			ERROR_LOG("Error de rendimiento: " + std::to_string(ex.getCurrentFPS()) + " FPS");
		} catch (const GameLogicException& ex) {
			// Manejar excepciones de lógica del juego
			HANDLE_GAME_EXCEPTION(ex);
			ERROR_LOG("Error de lógica: esperado '" + ex.getExpectedValue() + "', actual '" + ex.getActualValue() + "'");
		} catch (const GameException& ex) {
			// Manejar cualquier otra excepción del juego
			HANDLE_GAME_EXCEPTION(ex);
			ERROR_LOG("Excepción del juego: " + std::string(ex.what()));
		} catch (const std::bad_alloc& ex) {
			// Manejar errores de memoria específicos
			HANDLE_STD_EXCEPTION(ex);
			ERROR_LOG("Error de asignación de memoria: " + std::string(ex.what()));
		} catch (const std::runtime_error& ex) {
			// Manejar errores de tiempo de ejecución
			HANDLE_STD_EXCEPTION(ex);
			ERROR_LOG("Error de tiempo de ejecución: " + std::string(ex.what()));
		} catch (const std::logic_error& ex) {
			// Manejar errores de lógica
			HANDLE_STD_EXCEPTION(ex);
			ERROR_LOG("Error de lógica: " + std::string(ex.what()));
		} catch (const std::exception& ex) {
			// Manejar cualquier otra excepción estándar
			HANDLE_STD_EXCEPTION(ex);
			ERROR_LOG("Excepción estándar: " + std::string(ex.what()));
		} catch (...) {
			// Manejar excepciones desconocidas
			HANDLE_UNKNOWN_EXCEPTION();
			ERROR_LOG("Excepción desconocida capturada en el main loop");
		}
	}


		// Shutdown Debug System
		INFO_LOG("Cerrando sistema de juego");
		DebugSystem::Shutdown();
		
		// destroy the window and cleanup the OpenGL context
		CloseWindow();
		return 0;
	
};
