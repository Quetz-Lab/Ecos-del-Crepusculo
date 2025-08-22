#pragma once
#include <string>
#include <vector>
#include <memory>
#include "raylib.h"

namespace Quetz_LabEDC
{
    // Forward declarations
    class Logger;
    class DebugConsole;
    class PerformanceMonitor;
    class ObjectInspector;
    class CommandProcessor;
    class GameObject;

    // Enum para niveles de logging
    enum LogLevel
    {
        DEBUG_LOG = 0,
        INFO_LOG = 1,
        WARNING_LOG = 2,
        ERROR_LOG = 3,
        CRITICAL_LOG = 4
    };

    // Estructura para entradas de log
    struct LogEntry
    {
        std::string message;
        LogLevel level;
        std::string timestamp;
        std::string file;
        int line;
        
        LogEntry(const std::string& msg, LogLevel lvl, const std::string& ts, 
                const std::string& f, int l)
            : message(msg), level(lvl), timestamp(ts), file(f), line(l) {}
    };

    // Estructura para datos de rendimiento
    struct PerformanceData
    {
        float currentFPS;
        float averageFPS;
        size_t memoryUsage;
        float frameTime;
        std::vector<float> fpsHistory;
        
        PerformanceData() : currentFPS(0.0f), averageFPS(0.0f), 
                           memoryUsage(0), frameTime(0.0f) {}
    };

    // Clase principal del sistema de debug (Singleton)
    class DebugSystem
    {
    private:
        static DebugSystem* instance;
        
        std::unique_ptr<Logger> logger;
        std::unique_ptr<DebugConsole> console;
        std::unique_ptr<PerformanceMonitor> perfMonitor;
        std::unique_ptr<ObjectInspector> inspector;
        std::unique_ptr<CommandProcessor> cmdProcessor;
        std::unique_ptr<class DebugConfig> config;
        
        bool isEnabled;
        bool isInitialized;

        // Constructor privado para singleton
        DebugSystem();
        
    public:
        // Destructor
        ~DebugSystem();
        
        // Eliminar constructor de copia y operador de asignación
        DebugSystem(const DebugSystem&) = delete;
        DebugSystem& operator=(const DebugSystem&) = delete;
        
        // Método para obtener la instancia singleton
        static DebugSystem& getInstance();
        
        // Métodos principales del sistema
        void initialize();
        void update();
        void draw();
        void shutdown();
        
        // Métodos de logging
        void logDebug(const std::string& message, const char* file = "", int line = 0);
        void logInfo(const std::string& message, const char* file = "", int line = 0);
        void logWarning(const std::string& message, const char* file = "", int line = 0);
        void logError(const std::string& message, const char* file = "", int line = 0);
        void logCritical(const std::string& message, const char* file = "", int line = 0);
        
        // Métodos de acceso a componentes
        Logger* getLogger() const { return logger.get(); }
        DebugConsole* getConsole() const { return console.get(); }
        PerformanceMonitor* getPerformanceMonitor() const { return perfMonitor.get(); }
        ObjectInspector* getObjectInspector() const { return inspector.get(); }
        CommandProcessor* getCommandProcessor() const { return cmdProcessor.get(); }
        class DebugConfig* getConfig() const { return config.get(); }
        
        // Estado del sistema
        bool getIsEnabled() const { return isEnabled; }
        void setEnabled(bool enabled) { isEnabled = enabled; }
        bool getIsInitialized() const { return isInitialized; }
        
        // Métodos estáticos para facilitar el uso
        static void Initialize() { getInstance().initialize(); }
        static void Update() { getInstance().update(); }
        static void Draw() { getInstance().draw(); }
        static void Shutdown() { getInstance().shutdown(); }
    };
}

// Macros de conveniencia para logging
#define DEBUG_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logDebug(msg, __FILE__, __LINE__)
#define INFO_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logInfo(msg, __FILE__, __LINE__)
#define WARN_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logWarning(msg, __FILE__, __LINE__)
#define ERROR_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logError(msg, __FILE__, __LINE__)
#define CRITICAL_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logCritical(msg, __FILE__, __LINE__)