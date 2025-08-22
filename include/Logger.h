#pragma once
#include "DebugSystem.h"
#include <fstream>
#include <deque>
#include <string>
#include <vector>

namespace Quetz_LabEDC
{
    class Logger
    {
    private:
        std::deque<LogEntry> logHistory;
        LogLevel minLevel;
        std::ofstream logFile;
        std::string currentLogFileName;
        size_t maxHistorySize;
        size_t maxFileSize;
        bool fileLoggingEnabled;
        
        // Métodos privados
        std::string getCurrentTimestamp() const;
        std::string formatLogEntry(const LogEntry& entry) const;
        std::string getLevelString(LogLevel level) const;
        void checkAndRotateLog();
        std::string generateLogFileName() const;
        
    public:
        Logger();
        ~Logger();
        
        // Configuración
        void setMinLevel(LogLevel level) { minLevel = level; }
        LogLevel getMinLevel() const { return minLevel; }
        void setMaxHistorySize(size_t size) { maxHistorySize = size; }
        void setMaxFileSize(size_t size) { maxFileSize = size; }
        void setFileLoggingEnabled(bool enabled);
        
        // Logging principal
        void log(const std::string& message, LogLevel level, 
                const char* file = "", int line = 0);
        
        // Acceso al historial
        std::vector<LogEntry> getRecentLogs(size_t count) const;
        std::vector<LogEntry> getAllLogs() const;
        size_t getLogCount() const { return logHistory.size(); }
        
        // Manejo de archivos
        void saveToFile();
        void rotateLogs();
        void clearHistory();
        
        // Estado
        bool isFileLoggingEnabled() const { return fileLoggingEnabled; }
        std::string getCurrentLogFile() const { return currentLogFileName; }
    };
}