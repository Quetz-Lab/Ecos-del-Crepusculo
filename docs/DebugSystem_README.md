# Sistema de Debug de Errores - Ecos del Crepúsculo

## Descripción General

El Sistema de Debug de Errores es una herramienta completa de diagnóstico y depuración integrada en el juego "Ecos del Crepúsculo". Proporciona logging multinivel, consola visual, monitoreo de rendimiento, inspección de objetos y comandos de debug.

## Características Principales

### 🔍 Sistema de Logging
- **5 niveles de severidad**: DEBUG, INFO, WARNING, ERROR, CRITICAL
- **Logging a archivos** con rotación automática
- **Historial en memoria** configurable
- **Timestamps precisos** con información de archivo y línea

### 🖥️ Consola Visual
- **Activación con F1**
- **Visualización en tiempo real** de logs
- **Ejecución de comandos** interactiva
- **Interfaz no intrusiva** que no afecta el gameplay

### 📊 Monitor de Rendimiento
- **Tracking de FPS** en tiempo real
- **Monitoreo de memoria** del proceso
- **Detección automática** de problemas de rendimiento
- **Overlay visual** con gráficos de FPS

### 🔧 Inspector de Objetos
- **Inspección en tiempo real** de GameObjects
- **Modificación de propiedades** durante la ejecución
- **Selección por clic** o comando
- **Soporte para tipos específicos** (Player, Enemy, SideKick, etc.)

### ⚙️ Sistema de Configuración
- **Configuración persistente** en archivo INI
- **Modificación en tiempo real** desde la consola
- **Valores por defecto** sensatos
- **Aplicación automática** de cambios

## Instalación y Configuración

### Requisitos
- C++17 o superior
- Raylib (para el juego principal)
- Sistema de archivos compatible con std::filesystem

### Integración en el Proyecto

1. **Incluir headers necesarios**:
```cpp
#include "DebugSystem.h"
using namespace Quetz_LabEDC;
```

2. **Inicializar en main.cpp**:
```cpp
// Después de InitWindow()
DebugSystem::Initialize();
INFO_LOG("Sistema de juego inicializado");
```

3. **Actualizar en el game loop**:
```cpp
while (!WindowShouldClose()) {
    // Update Debug System
    DebugSystem::Update();
    
    // ... resto del código de update ...
    
    BeginDrawing();
    // ... código de renderizado ...
    
    // Draw Debug System (debe ser último)
    DebugSystem::Draw();
    EndDrawing();
}
```

4. **Limpiar al cerrar**:
```cpp
DebugSystem::Shutdown();
CloseWindow();
```

## Uso del Sistema

### Macros de Logging
```cpp
DEBUG_LOG("Información de depuración detallada");
INFO_LOG("Información general del juego");
WARN_LOG("Advertencia: rendimiento bajo");
ERROR_LOG("Error recuperable en el sistema");
CRITICAL_LOG("Error crítico que requiere atención");
```

### Controles de Teclado
- **F1**: Toggle consola de debug
- **F2**: Toggle inspector de objetos
- **F3**: Toggle overlay de rendimiento

### Comandos de Consola

#### Comandos Básicos
```
help                    - Muestra lista de comandos
help <comando>          - Ayuda específica para un comando
clear                   - Limpia la consola
exit                    - Cierra la consola
```

#### Información del Sistema
```
fps                     - Muestra información de rendimiento
memory                  - Muestra uso de memoria
objects                 - Lista todos los GameObjects
```

#### Manipulación de Objetos
```
inspect <nombre>        - Inspecciona un objeto específico
teleport <x> <y>        - Teletransporta al jugador
spawn enemy [x] [y]     - Crea un enemigo
spawn sidekick [x] [y]  - Crea un sidekick
```

#### Configuración
```
config                  - Muestra toda la configuración
config <clave>          - Muestra valor específico
config <clave> <valor>  - Modifica configuración
config save             - Guarda configuración
config reload           - Recarga configuración
config reset            - Restaura valores por defecto
```

## Configuración Avanzada

### Archivo debug_config.ini

El sistema genera automáticamente un archivo de configuración con las siguientes secciones:

```ini
[logger]
min_level = 0
max_history_size = 1000
max_file_size = 10485760
file_logging_enabled = true

[console]
max_display_lines = 20
font_size = 16
width = 800
height = 400
background_alpha = 200

[performance]
show_overlay = false
low_fps_threshold = 30.0
critical_fps_threshold = 15.0
max_history_size = 60

[inspector]
window_width = 300
window_height = 400
font_size = 14

[debug]
enabled = true
```

### Personalización de Colores y UI

```cpp
// Personalizar colores de la consola
auto* console = DebugSystem::getInstance().getConsole();
console->setColors({0, 0, 0, 180}, WHITE, YELLOW);

// Personalizar posición del inspector
auto* inspector = DebugSystem::getInstance().getObjectInspector();
inspector->setWindowPosition({50, 50});
```

## Optimización de Rendimiento

### Configuración para Release
```cpp
#ifdef RELEASE_BUILD
    // Deshabilitar debug en builds de release
    DebugSystem::getInstance().setEnabled(false);
#endif
```

### Configuración de Logging Eficiente
```cpp
// Solo errores y críticos en producción
auto* logger = DebugSystem::getInstance().getLogger();
logger->setMinLevel(LOG_ERROR);
logger->setFileLoggingEnabled(false);
```

### Lazy Loading
El sistema implementa lazy loading automático:
- Los componentes se inicializan solo cuando se necesitan
- El renderizado se omite cuando los componentes no están visibles
- La actualización se optimiza basada en el estado del sistema

## Ejemplos de Uso Común

### Debugging de Gameplay
```cpp
// En el código del jugador
void Player::takeDamage(int damage) {
    health -= damage;
    DEBUG_LOG("Jugador recibió " + std::to_string(damage) + " de daño. Salud: " + std::to_string(health));
    
    if (health <= 0) {
        ERROR_LOG("Jugador murió!");
    }
}
```

### Monitoreo de Rendimiento
```cpp
// Detectar problemas de rendimiento automáticamente
void GameLoop() {
    auto* perfMonitor = DebugSystem::getInstance().getPerformanceMonitor();
    
    if (perfMonitor->isPerformanceCritical()) {
        CRITICAL_LOG("Rendimiento crítico detectado!");
        // Reducir calidad gráfica automáticamente
    }
}
```

### Comandos Personalizados
```cpp
// Registrar comando personalizado
auto* cmdProcessor = DebugSystem::getInstance().getCommandProcessor();
cmdProcessor->registerCommand("godmode", "Activa modo invencible", "godmode [on|off]",
    [](const std::vector<std::string>& args) {
        // Implementar lógica de god mode
        bool enable = (args.size() > 1 && args[1] == "on");
        // ... código para activar/desactivar god mode ...
    });
```

## Solución de Problemas

### Problemas Comunes

1. **El sistema no se inicializa**
   - Verificar que se llame `DebugSystem::Initialize()` después de `InitWindow()`
   - Comprobar permisos de escritura para archivos de log

2. **La consola no aparece**
   - Verificar que se presione F1
   - Comprobar que el sistema esté habilitado en la configuración

3. **Logs no se guardan en archivo**
   - Verificar permisos de escritura en el directorio `logs/`
   - Comprobar configuración `logger.file_logging_enabled`

4. **Rendimiento afectado**
   - Reducir `logger.max_history_size`
   - Aumentar `logger.min_level` para filtrar logs
   - Deshabilitar componentes no necesarios

### Logs de Diagnóstico

El sistema genera logs automáticos para ayudar en el diagnóstico:
```
[INFO] Sistema de Debug inicializado correctamente
[INFO] Configuración cargada desde: debug_config.ini
[WARN] Rendimiento BAJO detectado - FPS: 25
[ERROR] Error inicializando componente: ...
```

## Contribución y Extensión

### Agregar Nuevos Comandos
```cpp
void registerCustomCommands() {
    auto* cmdProcessor = DebugSystem::getInstance().getCommandProcessor();
    
    cmdProcessor->registerCommand("mycommand", "Descripción", "mycommand <arg>",
        [](const std::vector<std::string>& args) {
            // Implementación del comando
        });
}
```

### Extender el Inspector
```cpp
// En ObjectInspector.cpp, método updateProperties()
if (MyCustomObject* customObj = dynamic_cast<MyCustomObject*>(selectedObject)) {
    properties.emplace_back("Custom Property", 
                          std::to_string(customObj->customValue), 
                          "float", true);
}
```

## Licencia y Créditos

Sistema desarrollado para "Ecos del Crepúsculo" como parte del sistema de debugging integrado.

---

Para más información o reportar problemas, consulta la documentación del proyecto principal.