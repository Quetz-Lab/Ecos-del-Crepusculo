#include "DebugConfig.h"
#include "DebugSystem.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace Quetz_LabEDC
{
    DebugConfig::DebugConfig()
        : configFilePath("debug_config.ini")
    {
        setDefaultValues();
    }
    
    DebugConfig::~DebugConfig()
    {
        // Guardar configuración al destruir
        saveToFile();
    }
    
    void DebugConfig::setDefaultValues()
    {
        // Configuración del Logger
        configValues["logger.min_level"] = "0"; // LOG_DEBUG
        configValues["logger.max_history_size"] = "1000";
        configValues["logger.max_file_size"] = "10485760"; // 10MB
        configValues["logger.file_logging_enabled"] = "true";
        
        // Configuración de la Consola
        configValues["console.max_display_lines"] = "20";
        configValues["console.font_size"] = "16";
        configValues["console.width"] = "800";
        configValues["console.height"] = "400";
        configValues["console.background_alpha"] = "200";
        
        // Configuración del Monitor de Rendimiento
        configValues["performance.show_overlay"] = "false";
        configValues["performance.low_fps_threshold"] = "30.0";
        configValues["performance.critical_fps_threshold"] = "15.0";
        configValues["performance.max_history_size"] = "60";
        configValues["performance.overlay_font_size"] = "16";
        
        // Configuración del Inspector
        configValues["inspector.window_width"] = "300";
        configValues["inspector.window_height"] = "400";
        configValues["inspector.font_size"] = "14";
        configValues["inspector.auto_update"] = "true";
        
        // Configuración de Comandos
        configValues["commands.max_history_size"] = "50";
        configValues["commands.case_sensitive"] = "false";
        
        // Configuración General
        configValues["debug.enabled"] = "true";
        configValues["debug.toggle_console_key"] = "F1";
        configValues["debug.toggle_inspector_key"] = "F2";
        configValues["debug.toggle_performance_key"] = "F3";
        
        // Configuración de Compilación
#ifdef _DEBUG
        configValues["build.debug_mode"] = "true";
#else
        configValues["build.debug_mode"] = "false";
#endif
    }
    
    bool DebugConfig::loadFromFile(const std::string& filePath)
    {
        configFilePath = filePath;
        std::ifstream file(filePath);
        
        if (!file.is_open())
        {
            std::cout << "Archivo de configuración no encontrado: " << filePath << std::endl;
            std::cout << "Usando configuración por defecto" << std::endl;
            return false;
        }
        
        std::string line;
        while (std::getline(file, line))
        {
            parseConfigLine(line);
        }
        
        file.close();
        std::cout << "Configuración cargada desde: " << filePath << std::endl;
        return true;
    }
    
    bool DebugConfig::saveToFile(const std::string& filePath)
    {
        std::string saveFilePath = filePath.empty() ? configFilePath : filePath;
        std::ofstream file(saveFilePath);
        
        if (!file.is_open())
        {
            std::cerr << "Error: No se pudo guardar configuración en: " << saveFilePath << std::endl;
            return false;
        }
        
        file << "# Configuración del Sistema de Debug - Ecos del Crepúsculo\n";
        file << "# Generado automáticamente\n\n";
        
        // Agrupar configuraciones por sección
        std::map<std::string, std::map<std::string, std::string>> sections;
        
        for (const auto& pair : configValues)
        {
            size_t dotPos = pair.first.find('.');
            if (dotPos != std::string::npos)
            {
                std::string section = pair.first.substr(0, dotPos);
                std::string key = pair.first.substr(dotPos + 1);
                sections[section][key] = pair.second;
            }
            else
            {
                sections["general"][pair.first] = pair.second;
            }
        }
        
        // Escribir secciones
        for (const auto& section : sections)
        {
            file << "[" << section.first << "]\n";
            for (const auto& keyValue : section.second)
            {
                file << keyValue.first << " = " << keyValue.second << "\n";
            }
            file << "\n";
        }
        
        file.close();
        std::cout << "Configuración guardada en: " << saveFilePath << std::endl;
        return true;
    }
    
    void DebugConfig::parseConfigLine(const std::string& line)
    {
        std::string trimmedLine = trim(line);
        
        // Ignorar líneas vacías y comentarios
        if (trimmedLine.empty() || trimmedLine[0] == '#' || trimmedLine[0] == '[')
        {
            return;
        }
        
        // Buscar el separador '='
        size_t equalPos = trimmedLine.find('=');
        if (equalPos == std::string::npos)
        {
            return;
        }
        
        std::string key = trim(trimmedLine.substr(0, equalPos));
        std::string value = trim(trimmedLine.substr(equalPos + 1));
        
        if (!key.empty())
        {
            configValues[key] = value;
        }
    }
    
    std::string DebugConfig::trim(const std::string& str)
    {
        size_t start = str.find_first_not_of(" \t\r\n");
        if (start == std::string::npos)
        {
            return "";
        }
        
        size_t end = str.find_last_not_of(" \t\r\n");
        return str.substr(start, end - start + 1);
    }
    
    std::string DebugConfig::getString(const std::string& key, const std::string& defaultValue) const
    {
        auto it = configValues.find(key);
        return (it != configValues.end()) ? it->second : defaultValue;
    }
    
    int DebugConfig::getInt(const std::string& key, int defaultValue) const
    {
        auto it = configValues.find(key);
        if (it != configValues.end())
        {
            try
            {
                return std::stoi(it->second);
            }
            catch (const std::exception&)
            {
                return defaultValue;
            }
        }
        return defaultValue;
    }
    
    float DebugConfig::getFloat(const std::string& key, float defaultValue) const
    {
        auto it = configValues.find(key);
        if (it != configValues.end())
        {
            try
            {
                return std::stof(it->second);
            }
            catch (const std::exception&)
            {
                return defaultValue;
            }
        }
        return defaultValue;
    }
    
    bool DebugConfig::getBool(const std::string& key, bool defaultValue) const
    {
        auto it = configValues.find(key);
        if (it != configValues.end())
        {
            std::string value = it->second;
            std::transform(value.begin(), value.end(), value.begin(), ::tolower);
            return (value == "true" || value == "1" || value == "yes");
        }
        return defaultValue;
    }
    
    void DebugConfig::setString(const std::string& key, const std::string& value)
    {
        configValues[key] = value;
    }
    
    void DebugConfig::setInt(const std::string& key, int value)
    {
        configValues[key] = std::to_string(value);
    }
    
    void DebugConfig::setFloat(const std::string& key, float value)
    {
        configValues[key] = std::to_string(value);
    }
    
    void DebugConfig::setBool(const std::string& key, bool value)
    {
        configValues[key] = value ? "true" : "false";
    }
    
    void DebugConfig::applyToDebugSystem()
    {
        auto& debugSystem = DebugSystem::getInstance();
        
        // Aplicar configuración del sistema general
        debugSystem.setEnabled(getBool("debug.enabled", true));
        
        // Aplicar configuración del Logger
        if (auto* logger = debugSystem.getLogger())
        {
            logger->setMinLevel(static_cast<LogLevel>(getInt("logger.min_level", 0)));
            logger->setMaxHistorySize(getInt("logger.max_history_size", 1000));
            logger->setMaxFileSize(getInt("logger.max_file_size", 10485760));
            logger->setFileLoggingEnabled(getBool("logger.file_logging_enabled", true));
        }
        
        // Aplicar configuración de la Consola
        if (auto* console = debugSystem.getConsole())
        {
            console->setMaxDisplayLines(getInt("console.max_display_lines", 20));
            console->setFontSize(getInt("console.font_size", 16));
            console->setSize(getFloat("console.width", 800.0f), getFloat("console.height", 400.0f));
            
            // Configurar colores con alpha
            int alpha = getInt("console.background_alpha", 200);
            console->setColors({0, 0, 0, static_cast<unsigned char>(alpha)}, WHITE, YELLOW);
        }
        
        // Aplicar configuración del Monitor de Rendimiento
        if (auto* perfMonitor = debugSystem.getPerformanceMonitor())
        {
            perfMonitor->setOverlayVisible(getBool("performance.show_overlay", false));
            perfMonitor->setLowFpsThreshold(getFloat("performance.low_fps_threshold", 30.0f));
            perfMonitor->setCriticalFpsThreshold(getFloat("performance.critical_fps_threshold", 15.0f));
            perfMonitor->setMaxHistorySize(getInt("performance.max_history_size", 60));
        }
        
        // Aplicar configuración del Inspector
        if (auto* inspector = debugSystem.getObjectInspector())
        {
            inspector->setWindowSize({
                getFloat("inspector.window_width", 300.0f),
                getFloat("inspector.window_height", 400.0f)
            });
            inspector->setFontSize(getInt("inspector.font_size", 14));
        }
        
        // Aplicar configuración de Comandos
        if (auto* cmdProcessor = debugSystem.getCommandProcessor())
        {
            cmdProcessor->setMaxHistorySize(getInt("commands.max_history_size", 50));
        }
        
        std::cout << "Configuración aplicada al sistema de debug" << std::endl;
    }
    
    void DebugConfig::resetToDefaults()
    {
        configValues.clear();
        setDefaultValues();
        std::cout << "Configuración restablecida a valores por defecto" << std::endl;
    }
}