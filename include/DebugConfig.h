#pragma once
#include "DebugSystem.h"
#include <string>
#include <map>

namespace Quetz_LabEDC
{
    class DebugConfig
    {
    private:
        std::map<std::string, std::string> configValues;
        std::string configFilePath;
        
        // Valores por defecto
        void setDefaultValues();
        
        // Parsing de archivos
        void parseConfigLine(const std::string& line);
        std::string trim(const std::string& str);
        
    public:
        DebugConfig();
        ~DebugConfig();
        
        // Carga y guardado
        bool loadFromFile(const std::string& filePath = "debug_config.ini");
        bool saveToFile(const std::string& filePath = "debug_config.ini");
        
        // Acceso a configuración
        std::string getString(const std::string& key, const std::string& defaultValue = "") const;
        int getInt(const std::string& key, int defaultValue = 0) const;
        float getFloat(const std::string& key, float defaultValue = 0.0f) const;
        bool getBool(const std::string& key, bool defaultValue = false) const;
        
        // Modificación de configuración
        void setString(const std::string& key, const std::string& value);
        void setInt(const std::string& key, int value);
        void setFloat(const std::string& key, float value);
        void setBool(const std::string& key, bool value);
        
        // Aplicar configuración al sistema de debug
        void applyToDebugSystem();
        
        // Obtener todas las configuraciones
        std::map<std::string, std::string> getAllConfig() const { return configValues; }
        
        // Resetear a valores por defecto
        void resetToDefaults();
    };
}