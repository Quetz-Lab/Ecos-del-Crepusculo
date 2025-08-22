# Documento de Diseño - Sistema de Debug de Errores

## Visión General

El sistema de debug de errores se integrará como un componente singleton en la arquitectura existente del juego "Ecos del Crepúsculo". Seguirá el patrón de diseño ya establecido por UISystem, proporcionando una interfaz unificada para logging, debugging visual y monitoreo de rendimiento.

## Arquitectura

### Componentes Principales

```
DebugSystem (Singleton)
├── Logger (Subsistema de Logging)
├── DebugConsole (Consola Visual)
├── PerformanceMonitor (Monitor de Rendimiento)
├── ObjectInspector (Inspector de GameObjects)
└── CommandProcessor (Procesador de Comandos)
```

### Integración con Sistema Existente

El sistema se integrará con:
- **GameObject**: Para inspección de objetos y estado
- **UISystem**: Para renderizado de elementos de debug
- **Player**: Para comandos específicos del jugador
- **Level**: Para información del nivel actual

## Componentes e Interfaces

### 1. DebugSystem (Clase Principal)

```cpp
class DebugSystem {
private:
    static DebugSystem* instance;
    Logger* logger;
    DebugConsole* console;
    PerformanceMonitor* perfMonitor;
    ObjectInspector* inspector;
    CommandProcessor* cmdProcessor;
    bool isEnabled;
    
public:
    static DebugSystem& getInstance();
    void initialize();
    void update();
    void draw();
    void shutdown();
    
    // Métodos de logging
    void logDebug(const std::string& message, const char* file, int line);
    void logInfo(const std::string& message, const char* file, int line);
    void logWarning(const std::string& message, const char* file, int line);
    void logError(const std::string& message, const char* file, int line);
    void logCritical(const std::string& message, const char* file, int line);
};
```

### 2. Logger (Subsistema de Logging)

```cpp
enum LogLevel {
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3,
    LOG_CRITICAL = 4
};

struct LogEntry {
    std::string message;
    LogLevel level;
    std::string timestamp;
    std::string file;
    int line;
};

class Logger {
private:
    std::vector<LogEntry> logHistory;
    LogLevel minLevel;
    std::ofstream logFile;
    size_t maxHistorySize;
    
public:
    void log(const std::string& message, LogLevel level, const char* file, int line);
    void setMinLevel(LogLevel level);
    std::vector<LogEntry> getRecentLogs(size_t count);
    void saveToFile();
    void rotateLogs();
};
```

### 3. DebugConsole (Consola Visual)

```cpp
class DebugConsole {
private:
    bool isVisible;
    std::vector<std::string> displayLines;
    std::string inputBuffer;
    Rectangle consoleRect;
    Color backgroundColor;
    Color textColor;
    
public:
    void toggle();
    void addLine(const std::string& line);
    void processInput(const std::string& input);
    void draw();
    void update();
    bool isActive() const;
};
```

### 4. PerformanceMonitor (Monitor de Rendimiento)

```cpp
struct PerformanceData {
    float currentFPS;
    float averageFPS;
    size_t memoryUsage;
    float frameTime;
    std::vector<float> fpsHistory;
};

class PerformanceMonitor {
private:
    PerformanceData perfData;
    bool showOverlay;
    
public:
    void update();
    void draw();
    PerformanceData getCurrentData();
    void toggleOverlay();
    void checkPerformanceThresholds();
};
```

### 5. ObjectInspector (Inspector de GameObjects)

```cpp
class ObjectInspector {
private:
    GameObject* selectedObject;
    bool isActive;
    Rectangle inspectorWindow;
    
public:
    void selectObject(GameObject* obj);
    void drawInspector();
    void handleInput();
    void updateObjectProperty(const std::string& property, const std::string& value);
    bool isInspecting() const;
};
```

### 6. CommandProcessor (Procesador de Comandos)

```cpp
class CommandProcessor {
private:
    std::map<std::string, std::function<void(const std::vector<std::string>&)>> commands;
    
public:
    void registerCommand(const std::string& name, std::function<void(const std::vector<std::string>&)> func);
    void executeCommand(const std::string& input);
    std::vector<std::string> getAvailableCommands();
    void initializeDefaultCommands();
};
```

## Modelos de Datos

### Estructura de Archivos de Log

```
logs/
├── game_YYYY-MM-DD_HH-MM-SS.log    // Log principal de la sesión
├── error_YYYY-MM-DD_HH-MM-SS.log   // Solo errores críticos
└── archived/                        // Logs archivados cuando superan el tamaño límite
    ├── game_YYYY-MM-DD_HH-MM-SS_001.log
    └── game_YYYY-MM-DD_HH-MM-SS_002.log
```

### Formato de Entrada de Log

```
[TIMESTAMP] [LEVEL] [FILE:LINE] MESSAGE
Ejemplo: [2024-01-15 14:30:25] [ERROR] [Player.cpp:45] Player health below zero: -5
```

### Comandos de Debug Predeterminados

```cpp
// Comandos básicos que se registrarán automáticamente
"help"           - Mostrar lista de comandos
"clear"          - Limpiar consola
"fps"            - Mostrar información de FPS
"memory"         - Mostrar uso de memoria
"objects"        - Listar todos los GameObjects
"inspect <name>" - Inspeccionar objeto específico
"teleport <x> <y>" - Teletransportar jugador
"spawn <type>"   - Crear nuevo objeto
"save"           - Guardar estado actual
"load"           - Cargar estado guardado
```

## Manejo de Errores

### Estrategias de Recuperación

1. **Errores de Logging**: Si falla el sistema de archivos, continuar con logging en memoria
2. **Errores de Consola**: Si falla el renderizado, desactivar consola pero mantener logging
3. **Errores de Inspector**: Si falla la inspección, mostrar mensaje de error pero continuar
4. **Errores de Comandos**: Mostrar mensaje de error descriptivo y sugerencias

### Validación de Entrada

- Validar comandos antes de ejecutar
- Sanitizar entrada de usuario para prevenir crashes
- Verificar existencia de objetos antes de inspeccionar
- Validar rangos de valores para propiedades modificables

## Estrategia de Pruebas

### Pruebas Unitarias

1. **Logger**: Verificar correcta categorización y formato de mensajes
2. **CommandProcessor**: Probar ejecución de comandos válidos e inválidos
3. **PerformanceMonitor**: Verificar cálculos de FPS y detección de umbrales
4. **ObjectInspector**: Probar selección y modificación de propiedades

### Pruebas de Integración

1. **Consola + Logger**: Verificar que los mensajes aparezcan correctamente
2. **Inspector + GameObjects**: Probar inspección de diferentes tipos de objetos
3. **Comandos + Sistema de Juego**: Verificar que los comandos afecten el estado correctamente
4. **Rendimiento**: Probar bajo diferentes cargas de trabajo

### Pruebas de Rendimiento

1. **Overhead de Logging**: Medir impacto en FPS con diferentes niveles de logging
2. **Memoria**: Verificar que el historial de logs no cause memory leaks
3. **Renderizado**: Asegurar que la consola no afecte significativamente el rendimiento

## Consideraciones de Implementación

### Macros de Conveniencia

```cpp
#define DEBUG_LOG(msg) DebugSystem::getInstance().logDebug(msg, __FILE__, __LINE__)
#define INFO_LOG(msg) DebugSystem::getInstance().logInfo(msg, __FILE__, __LINE__)
#define WARN_LOG(msg) DebugSystem::getInstance().logWarning(msg, __FILE__, __LINE__)
#define ERROR_LOG(msg) DebugSystem::getInstance().logError(msg, __FILE__, __LINE__)
#define CRITICAL_LOG(msg) DebugSystem::getInstance().logCritical(msg, __FILE__, __LINE__)
```

### Configuración

- Archivo de configuración `debug_config.ini` para ajustes persistentes
- Variables de entorno para configuración de desarrollo
- Compilación condicional para builds de release/debug

### Integración con Raylib

- Usar funciones de Raylib para renderizado de texto y formas
- Integrar con el sistema de input existente
- Respetar el ciclo de vida de texturas y recursos de Raylib

### Optimizaciones

- Lazy loading de componentes no críticos
- Pooling de objetos para evitar allocaciones frecuentes
- Renderizado condicional basado en visibilidad
- Compresión de logs antiguos para ahorrar espacio