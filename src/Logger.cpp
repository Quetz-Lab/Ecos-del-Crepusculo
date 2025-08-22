#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>

// ==================== SISTEMA DE LOGGING AVANZADO ====================

namespace Quetz_LabEDC {

    enum class LogLevel {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3,
        CRITICAL = 4
    };

    struct LogEntry {
        std::chrono::system_clock::time_point timestamp;
        LogLevel level;
        std::string message;
        std::string file;
        int line;
        std::string function;
        
        LogEntry(LogLevel lvl, const std::string& msg, const std::string& f = "", int l = 0, const std::string& func = "")
            : timestamp(std::chrono::system_clock::now()), level(lvl), message(msg), file(f), line(l), function(func) {}
    };

    class Logger {
    private:
        static Logger* instance;
        std::vector<LogEntry> logHistory;
        LogLevel minLogLevel;
        bool writeToFile;
        std::string logFileName;
        size_t maxLogEntries;
        bool enableConsoleOutput;
        
        Logger() : minLogLevel(LogLevel::DEBUG), writeToFile(true), 
                  logFileName("debug_log.txt"), maxLogEntries(1000), enableConsoleOutput(true) {
            // Crear directorio de logs si no existe
            std::filesystem::create_directories("logs");
            logFileName = "logs/" + logFileName;
        }

    public:
        static Logger& getInstance() {
            if (!instance) {
                instance = new Logger();
            }
            return *instance;
        }

        void setMinLogLevel(LogLevel level) {
            minLogLevel = level;
        }

        void setWriteToFile(bool enable) {
            writeToFile = enable;
        }

        void setConsoleOutput(bool enable) {
            enableConsoleOutput = enable;
        }

        void setMaxLogEntries(size_t max) {
            maxLogEntries = max;
        }

        std::string levelToString(LogLevel level) {
            switch (level) {
                case LogLevel::DEBUG: return "DEBUG";
                case LogLevel::INFO: return "INFO";
                case LogLevel::WARNING: return "WARNING";
                case LogLevel::ERROR: return "ERROR";
                case LogLevel::CRITICAL: return "CRITICAL";
                default: return "UNKNOWN";
            }
        }

        std::string formatTimestamp(const std::chrono::system_clock::time_point& timestamp) {
            auto time_t = std::chrono::system_clock::to_time_t(timestamp);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                timestamp.time_since_epoch()) % 1000;
            
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
            ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
            return ss.str();
        }

        void log(LogLevel level, const std::string& message, const std::string& file = "", int line = 0, const std::string& function = "") {
            if (level < minLogLevel) {
                return;
            }

            LogEntry entry(level, message, file, line, function);
            
            // Agregar al historial
            logHistory.push_back(entry);
            
            // Mantener límite de entradas
            if (logHistory.size() > maxLogEntries) {
                logHistory.erase(logHistory.begin());
            }

            // Formatear mensaje
            std::string formattedMessage = formatLogEntry(entry);

            // Salida a consola
            if (enableConsoleOutput) {
                std::cout << formattedMessage << std::endl;
            }

            // Escribir a archivo
            if (writeToFile) {
                writeToLogFile(formattedMessage);
            }
        }

        std::string formatLogEntry(const LogEntry& entry) {
            std::stringstream ss;
            ss << "[" << formatTimestamp(entry.timestamp) << "] ";
            ss << "[" << levelToString(entry.level) << "] ";
            ss << entry.message;
            
            if (!entry.file.empty()) {
                // Extraer solo el nombre del archivo, no la ruta completa
                std::string fileName = entry.file;
                size_t lastSlash = fileName.find_last_of("/\\");
                if (lastSlash != std::string::npos) {
                    fileName = fileName.substr(lastSlash + 1);
                }
                ss << " (" << fileName << ":" << entry.line << ")";
            }
            
            if (!entry.function.empty()) {
                ss << " [" << entry.function << "]";
            }
            
            return ss.str();
        }

        void writeToLogFile(const std::string& message) {
            try {
                std::ofstream logFile(logFileName, std::ios::app);
                if (logFile.is_open()) {
                    logFile << message << std::endl;
                    logFile.close();
                }
            } catch (const std::exception& e) {
                // Si no podemos escribir al log, al menos mostrarlo en consola
                std::cerr << "Error escribiendo al log file: " << e.what() << std::endl;
            }
        }

        // Métodos de conveniencia
        void debug(const std::string& message, const std::string& file = "", int line = 0) {
            log(LogLevel::DEBUG, message, file, line);
        }

        void info(const std::string& message, const std::string& file = "", int line = 0) {
            log(LogLevel::INFO, message, file, line);
        }

        void warning(const std::string& message, const std::string& file = "", int line = 0) {
            log(LogLevel::WARNING, message, file, line);
        }

        void error(const std::string& message, const std::string& file = "", int line = 0) {
            log(LogLevel::ERROR, message, file, line);
        }

        void critical(const std::string& message, const std::string& file = "", int line = 0) {
            log(LogLevel::CRITICAL, message, file, line);
        }

        // Obtener historial de logs
        const std::vector<LogEntry>& getLogHistory() const {
            return logHistory;
        }

        // Limpiar historial
        void clearHistory() {
            logHistory.clear();
        }

        // Obtener logs por nivel
        std::vector<LogEntry> getLogsByLevel(LogLevel level) const {
            std::vector<LogEntry> filtered;
            for (const auto& entry : logHistory) {
                if (entry.level == level) {
                    filtered.push_back(entry);
                }
            }
            return filtered;
        }

        // Obtener estadísticas de logs
        struct LogStats {
            size_t debugCount = 0;
            size_t infoCount = 0;
            size_t warningCount = 0;
            size_t errorCount = 0;
            size_t criticalCount = 0;
            size_t totalCount = 0;
        };

        LogStats getLogStats() const {
            LogStats stats;
            for (const auto& entry : logHistory) {
                switch (entry.level) {
                    case LogLevel::DEBUG: stats.debugCount++; break;
                    case LogLevel::INFO: stats.infoCount++; break;
                    case LogLevel::WARNING: stats.warningCount++; break;
                    case LogLevel::ERROR: stats.errorCount++; break;
                    case LogLevel::CRITICAL: stats.criticalCount++; break;
                }
                stats.totalCount++;
            }
            return stats;
        }

        // Rotar logs cuando el archivo sea muy grande
        void rotateLogFile() {
            try {
                if (std::filesystem::exists(logFileName)) {
                    auto fileSize = std::filesystem::file_size(logFileName);
                    if (fileSize > 10 * 1024 * 1024) { // 10MB
                        std::string backupName = logFileName + ".old";
                        std::filesystem::rename(logFileName, backupName);
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "Error rotando log file: " << e.what() << std::endl;
            }
        }

        // Exportar logs a formato específico
        void exportLogs(const std::string& filename, LogLevel minLevel = LogLevel::DEBUG) {
            try {
                std::ofstream exportFile(filename);
                if (exportFile.is_open()) {
                    exportFile << "=== LOG EXPORT ===" << std::endl;
                    exportFile << "Generated: " << formatTimestamp(std::chrono::system_clock::now()) << std::endl;
                    exportFile << "Min Level: " << levelToString(minLevel) << std::endl;
                    exportFile << "==================" << std::endl << std::endl;

                    for (const auto& entry : logHistory) {
                        if (entry.level >= minLevel) {
                            exportFile << formatLogEntry(entry) << std::endl;
                        }
                    }
                    exportFile.close();
                }
            } catch (const std::exception& e) {
                std::cerr << "Error exportando logs: " << e.what() << std::endl;
            }
        }

        ~Logger() {
            // Escribir estadísticas finales al cerrar
            if (writeToFile) {
                LogStats stats = getLogStats();
                std::string finalMessage = "=== SESSION END === Total logs: " + std::to_string(stats.totalCount) +
                                         " (DEBUG:" + std::to_string(stats.debugCount) +
                                         " INFO:" + std::to_string(stats.infoCount) +
                                         " WARNING:" + std::to_string(stats.warningCount) +
                                         " ERROR:" + std::to_string(stats.errorCount) +
                                         " CRITICAL:" + std::to_string(stats.criticalCount) + ")";
                writeToLogFile(finalMessage);
            }
        }
    };

    // Definición de la instancia estática
    Logger* Logger::instance = nullptr;

} // namespace Quetz_LabEDC

// Macros globales para facilitar el uso
#define LOG_DEBUG(msg) Quetz_LabEDC::Logger::getInstance().debug(msg, __FILE__, __LINE__)
#define LOG_INFO(msg) Quetz_LabEDC::Logger::getInstance().info(msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) Quetz_LabEDC::Logger::getInstance().warning(msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) Quetz_LabEDC::Logger::getInstance().error(msg, __FILE__, __LINE__)
#define LOG_CRITICAL(msg) Quetz_LabEDC::Logger::getInstance().critical(msg, __FILE__, __LINE__)