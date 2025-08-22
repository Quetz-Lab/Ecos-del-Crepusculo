#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <exception>
#include <stdexcept>
#include <typeinfo>
#include <chrono>
#include <sstream>
#include <fstream>
#include "raylib.h"

// ==================== SISTEMA DE DEBUG COMPLETO ====================

namespace Quetz_LabEDC {

    // Forward declaration del Logger
    class Logger;

    // ==================== CLASES DE EXCEPCIÓN PERSONALIZADAS ====================

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

    // ==================== SISTEMA DE DEBUG PRINCIPAL ====================

    class DebugSystem {
    private:
        static DebugSystem* instance;
        bool initialized;
        bool gameErrorOccurred;
        std::string errorMessage;
        std::string errorDetails;
        std::string errorFile;
        int errorLine;
        std::unique_ptr<GameException> lastException;
        
        // Configuración
        bool enableConsoleOutput;
        bool enableFileLogging;
        bool enableCriticalErrorWindows;
        
        // Estadísticas
        struct DebugStats {
            int totalErrors = 0;
            int criticalErrors = 0;
            int exceptionsHandled = 0;
            int recoveredErrors = 0;
        } stats;

        DebugSystem() : initialized(false), gameErrorOccurred(false), errorLine(0),
                       enableConsoleOutput(true), enableFileLogging(true), enableCriticalErrorWindows(true) {}

    public:
        static DebugSystem& getInstance() {
            if (!instance) {
                instance = new DebugSystem();
            }
            return *instance;
        }

        void initialize() {
            if (initialized) return;
            
            initialized = true;
            logInfo("Sistema de Debug inicializado", __FILE__, __LINE__);
            
            // Configurar el logger
            // Logger::getInstance().setMinLogLevel(LogLevel::DEBUG);
            // Logger::getInstance().setWriteToFile(enableFileLogging);
            // Logger::getInstance().setConsoleOutput(enableConsoleOutput);
        }

        void shutdown() {
            if (!initialized) return;
            
            logInfo("Cerrando Sistema de Debug", __FILE__, __LINE__);
            logInfo("Estadísticas finales - Errores: " + std::to_string(stats.totalErrors) + 
                   ", Críticos: " + std::to_string(stats.criticalErrors) + 
                   ", Excepciones: " + std::to_string(stats.exceptionsHandled) + 
                   ", Recuperados: " + std::to_string(stats.recoveredErrors), __FILE__, __LINE__);
            
            initialized = false;
        }

        // ==================== MÉTODOS DE LOGGING ====================

        void logDebug(const std::string& message, const char* file, int line) {
            if (enableConsoleOutput) {
                std::cout << "[DEBUG] " << message << " (" << extractFileName(file) << ":" << line << ")" << std::endl;
            }
        }

        void logInfo(const std::string& message, const char* file, int line) {
            if (enableConsoleOutput) {
                std::cout << "[INFO] " << message << " (" << extractFileName(file) << ":" << line << ")" << std::endl;
            }
        }

        void logWarning(const std::string& message, const char* file, int line) {
            if (enableConsoleOutput) {
                std::cout << "[WARNING] " << message << " (" << extractFileName(file) << ":" << line << ")" << std::endl;
            }
            stats.totalErrors++;
        }

        void logError(const std::string& message, const char* file, int line) {
            if (enableConsoleOutput) {
                std::cout << "[ERROR] " << message << " (" << extractFileName(file) << ":" << line << ")" << std::endl;
            }
            stats.totalErrors++;
        }

        void logCritical(const std::string& message, const char* file, int line) {
            if (enableConsoleOutput) {
                std::cout << "[CRITICAL] " << message << " (" << extractFileName(file) << ":" << line << ")" << std::endl;
            }
            stats.totalErrors++;
            stats.criticalErrors++;
        }

        // ==================== MANEJO DE ERRORES CRÍTICOS ====================

        void triggerCriticalError(const std::string& message, const std::string& details, const char* file, int line) {
            logCritical(message, file, line);
            
            if (enableCriticalErrorWindows) {
                gameErrorOccurred = true;
                errorMessage = message;
                errorDetails = details;
                errorFile = extractFileName(file);
                errorLine = line;
            }
        }

        // ==================== MANEJO DE EXCEPCIONES ====================

        void handleGameException(const GameException& ex) {
            logError("Excepción del juego capturada: " + std::string(ex.what()), ex.getFile().c_str(), ex.getLine());
            
            gameErrorOccurred = true;
            errorMessage = ex.getErrorType() + ": " + ex.what();
            errorDetails = ex.getDetailedMessage();
            errorFile = extractFileName(ex.getFile());
            errorLine = ex.getLine();
            
            // Guardar la excepción para referencia
            lastException = std::make_unique<GameException>(ex);
            stats.exceptionsHandled++;
        }

        void handleStandardException(const std::exception& ex, const char* file, int line) {
            logError("Excepción estándar capturada: " + std::string(ex.what()), file, line);
            
            gameErrorOccurred = true;
            errorMessage = "Excepción Estándar: " + std::string(ex.what());
            errorDetails = "Se ha producido una excepción estándar de C++.\n\n"
                          "Tipo: " + std::string(typeid(ex).name()) + "\n"
                          "Mensaje: " + std::string(ex.what()) + "\n"
                          "Ubicación: " + extractFileName(file) + ":" + std::to_string(line) + "\n\n"
                          "Esto puede indicar:\n"
                          "• Error de programación\n"
                          "• Condición inesperada\n"
                          "• Problema de recursos del sistema\n"
                          "• Corrupción de datos";
            errorFile = extractFileName(file);
            errorLine = line;
            stats.exceptionsHandled++;
        }

        void handleUnknownException(const char* file, int line) {
            logError("Excepción desconocida capturada", file, line);
            
            gameErrorOccurred = true;
            errorMessage = "Excepción Desconocida";
            errorDetails = "Se ha producido una excepción de tipo desconocido.\n\n"
                          "Ubicación: " + extractFileName(file) + ":" + std::to_string(line) + "\n\n"
                          "Esto puede indicar:\n"
                          "• Excepción personalizada no capturada\n"
                          "• Error de sistema de bajo nivel\n"
                          "• Corrupción de memoria\n"
                          "• Problema crítico del hardware\n\n"
                          "Se recomienda reiniciar el juego inmediatamente.";
            errorFile = extractFileName(file);
            errorLine = line;
            stats.exceptionsHandled++;
        }

        // ==================== GESTIÓN DE ESTADO DE ERROR ====================

        bool hasError() const {
            return gameErrorOccurred;
        }

        void clearError() {
            gameErrorOccurred = false;
            errorMessage.clear();
            errorDetails.clear();
            errorFile.clear();
            errorLine = 0;
            lastException.reset();
            stats.recoveredErrors++;
        }

        const std::string& getErrorMessage() const {
            return errorMessage;
        }

        const std::string& getErrorDetails() const {
            return errorDetails;
        }

        const std::string& getErrorFile() const {
            return errorFile;
        }

        int getErrorLine() const {
            return errorLine;
        }

        // ==================== CONFIGURACIÓN ====================

        void setConsoleOutput(bool enable) {
            enableConsoleOutput = enable;
        }

        void setFileLogging(bool enable) {
            enableFileLogging = enable;
        }

        void setCriticalErrorWindows(bool enable) {
            enableCriticalErrorWindows = enable;
        }

        // ==================== ESTADÍSTICAS ====================

        const DebugStats& getStats() const {
            return stats;
        }

        void resetStats() {
            stats = DebugStats();
        }

        // ==================== UTILIDADES ====================

        std::string extractFileName(const std::string& fullPath) const {
            size_t lastSlash = fullPath.find_last_of("/\\");
            if (lastSlash != std::string::npos) {
                return fullPath.substr(lastSlash + 1);
            }
            return fullPath;
        }

        std::string extractFileName(const char* fullPath) const {
            if (!fullPath) return "";
            return extractFileName(std::string(fullPath));
        }

        // ==================== MÉTODOS DE CONVENIENCIA ====================

        void update() {
            // Actualizar el sistema de debug si es necesario
            // Por ahora no hay lógica específica de actualización
        }

        void draw() {
            // Dibujar información de debug si es necesario
            // Por ahora no hay UI específica de debug
        }

        // ==================== EXPORTAR INFORMACIÓN ====================

        void exportDebugInfo(const std::string& filename) {
            try {
                std::ofstream file(filename);
                if (file.is_open()) {
                    file << "=== DEBUG SYSTEM REPORT ===" << std::endl;
                    file << "Timestamp: " << getCurrentTimestamp() << std::endl;
                    file << "Initialized: " << (initialized ? "Yes" : "No") << std::endl;
                    file << "Has Error: " << (gameErrorOccurred ? "Yes" : "No") << std::endl;
                    file << std::endl;
                    
                    file << "=== STATISTICS ===" << std::endl;
                    file << "Total Errors: " << stats.totalErrors << std::endl;
                    file << "Critical Errors: " << stats.criticalErrors << std::endl;
                    file << "Exceptions Handled: " << stats.exceptionsHandled << std::endl;
                    file << "Recovered Errors: " << stats.recoveredErrors << std::endl;
                    file << std::endl;
                    
                    if (gameErrorOccurred) {
                        file << "=== CURRENT ERROR ===" << std::endl;
                        file << "Message: " << errorMessage << std::endl;
                        file << "File: " << errorFile << ":" << errorLine << std::endl;
                        file << "Details: " << errorDetails << std::endl;
                    }
                    
                    file.close();
                }
            } catch (const std::exception& e) {
                logError("Error exportando información de debug: " + std::string(e.what()), __FILE__, __LINE__);
            }
        }

    private:
        std::string getCurrentTimestamp() {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
            return ss.str();
        }
    };

    // Definición de la instancia estática
    DebugSystem* DebugSystem::instance = nullptr;

} // namespace Quetz_LabEDC

// ==================== MACROS GLOBALES ====================

// Macros para logging
#define DEBUG_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logDebug(msg, __FILE__, __LINE__)
#define INFO_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logInfo(msg, __FILE__, __LINE__)
#define WARN_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logWarning(msg, __FILE__, __LINE__)
#define ERROR_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logError(msg, __FILE__, __LINE__)
#define CRITICAL_LOG(msg) Quetz_LabEDC::DebugSystem::getInstance().logCritical(msg, __FILE__, __LINE__)

// Macro para errores críticos
#define CRITICAL_ERROR(msg, details) Quetz_LabEDC::DebugSystem::getInstance().triggerCriticalError(msg, details, __FILE__, __LINE__)

// Macros para excepciones
#define THROW_GAME_EXCEPTION(msg) \
    throw Quetz_LabEDC::GameException(msg, __FILE__, __LINE__, __FUNCTION__)

#define THROW_CRITICAL_EXCEPTION(msg) \
    throw Quetz_LabEDC::CriticalGameException(msg, __FILE__, __LINE__, __FUNCTION__)

#define THROW_RESOURCE_EXCEPTION(msg, path) \
    throw Quetz_LabEDC::ResourceException(msg, path, __FILE__, __LINE__)

#define THROW_MEMORY_EXCEPTION(msg, requested, available) \
    throw Quetz_LabEDC::MemoryException(msg, requested, available, __FILE__, __LINE__)

#define THROW_PERFORMANCE_EXCEPTION(msg, current, minimum) \
    throw Quetz_LabEDC::PerformanceException(msg, current, minimum, __FILE__, __LINE__)

#define THROW_LOGIC_EXCEPTION(msg, expected, actual) \
    throw Quetz_LabEDC::GameLogicException(msg, expected, actual, __FILE__, __LINE__)

// Macros para manejo de excepciones
#define HANDLE_GAME_EXCEPTION(ex) Quetz_LabEDC::DebugSystem::getInstance().handleGameException(ex)
#define HANDLE_STD_EXCEPTION(ex) Quetz_LabEDC::DebugSystem::getInstance().handleStandardException(ex, __FILE__, __LINE__)
#define HANDLE_UNKNOWN_EXCEPTION() Quetz_LabEDC::DebugSystem::getInstance().handleUnknownException(__FILE__, __LINE__)

// Macro para ejecución segura
#define SAFE_EXECUTE(code) \
    try { \
        code; \
    } catch (const Quetz_LabEDC::GameException& ex) { \
        HANDLE_GAME_EXCEPTION(ex); \
    } catch (const std::exception& ex) { \
        HANDLE_STD_EXCEPTION(ex); \
    } catch (...) { \
        HANDLE_UNKNOWN_EXCEPTION(); \
    }