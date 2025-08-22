#pragma once

// Forward declarations
namespace Quetz_LabEDC {
    class DebugConfig;
    class Logger;
    class DebugConsole;
    class PerformanceMonitor;
}

namespace Quetz_LabEDC
{
    // Clase para optimizaciones específicas del sistema de debug
    class DebugOptimizations
    {
    public:
        // Configuraciones predefinidas para diferentes escenarios
        
        // Configuración para desarrollo activo
        static void configureForDevelopment()
        {
            auto& debugSystem = DebugSystem::getInstance();
            auto* config = debugSystem.getConfig();
            
            if (config)
            {
                // Logging completo para desarrollo
                config->setInt("logger.min_level", LOG_DEBUG);
                config->setBool("logger.file_logging_enabled", true);
                config->setInt("logger.max_history_size", 2000);
                
                // Consola más grande para desarrollo
                config->setFloat("console.width", 1000.0f);
                config->setFloat("console.height", 500.0f);
                config->setInt("console.max_display_lines", 30);
                
                // Performance overlay visible
                config->setBool("performance.show_overlay", true);
                config->setFloat("performance.low_fps_threshold", 45.0f);
                
                // Inspector más detallado
                config->setFloat("inspector.window_width", 350.0f);
                config->setFloat("inspector.window_height", 500.0f);
                
                config->applyToDebugSystem();
            }
        }
        
        // Configuración para testing/QA
        static void configureForTesting()
        {
            auto& debugSystem = DebugSystem::getInstance();
            auto* config = debugSystem.getConfig();
            
            if (config)
            {
                // Solo warnings y errores para testing
                config->setInt("logger.min_level", LOG_WARNING);
                config->setBool("logger.file_logging_enabled", true);
                config->setInt("logger.max_history_size", 500);
                
                // Consola más pequeña
                config->setFloat("console.width", 600.0f);
                config->setFloat("console.height", 300.0f);
                config->setInt("console.max_display_lines", 15);
                
                // Performance monitoring activo
                config->setBool("performance.show_overlay", false);
                config->setFloat("performance.low_fps_threshold", 30.0f);
                config->setFloat("performance.critical_fps_threshold", 20.0f);
                
                config->applyToDebugSystem();
            }
        }
        
        // Configuración para release/producción
        static void configureForRelease()
        {
            auto& debugSystem = DebugSystem::getInstance();
            auto* config = debugSystem.getConfig();
            
            if (config)
            {
                // Solo errores críticos en release
                config->setInt("logger.min_level", LOG_CRITICAL);
                config->setBool("logger.file_logging_enabled", false);
                config->setInt("logger.max_history_size", 100);
                
                // Deshabilitar componentes visuales
                config->setBool("performance.show_overlay", false);
                config->setBool("debug.enabled", false);
                
                config->applyToDebugSystem();
            }
        }
        
        // Configuración para debugging de rendimiento
        static void configureForPerformanceDebugging()
        {
            auto& debugSystem = DebugSystem::getInstance();
            auto* config = debugSystem.getConfig();
            
            if (config)
            {
                // Logging mínimo para no afectar rendimiento
                config->setInt("logger.min_level", LOG_ERROR);
                config->setBool("logger.file_logging_enabled", false);
                config->setInt("logger.max_history_size", 50);
                
                // Performance monitoring detallado
                config->setBool("performance.show_overlay", true);
                config->setFloat("performance.low_fps_threshold", 55.0f);
                config->setFloat("performance.critical_fps_threshold", 45.0f);
                config->setInt("performance.max_history_size", 120);
                
                // Consola mínima
                config->setFloat("console.width", 400.0f);
                config->setFloat("console.height", 200.0f);
                config->setInt("console.max_display_lines", 8);
                
                config->applyToDebugSystem();
            }
        }
        
        // Optimización automática basada en rendimiento actual
        static void autoOptimize()
        {
            auto& debugSystem = DebugSystem::getInstance();
            auto* perfMonitor = debugSystem.getPerformanceMonitor();
            auto* config = debugSystem.getConfig();
            
            if (!perfMonitor || !config) return;
            
            float currentFPS = perfMonitor->getCurrentFPS();
            
            if (currentFPS < 20.0f)
            {
                // Rendimiento crítico - deshabilitar casi todo
                config->setInt("logger.min_level", LOG_CRITICAL);
                config->setBool("logger.file_logging_enabled", false);
                config->setInt("logger.max_history_size", 20);
                config->setBool("performance.show_overlay", false);
                
                INFO_LOG("Auto-optimización: Modo rendimiento crítico activado");
            }
            else if (currentFPS < 40.0f)
            {
                // Rendimiento bajo - reducir logging
                config->setInt("logger.min_level", LOG_ERROR);
                config->setInt("logger.max_history_size", 100);
                
                INFO_LOG("Auto-optimización: Modo rendimiento bajo activado");
            }
            else if (currentFPS > 55.0f)
            {
                // Buen rendimiento - permitir más debugging
                config->setInt("logger.min_level", LOG_INFO);
                config->setInt("logger.max_history_size", 500);
                
                DEBUG_LOG("Auto-optimización: Modo rendimiento normal activado");
            }
            
            config->applyToDebugSystem();
        }
        
        // Configuración para memoria limitada
        static void configureForLowMemory()
        {
            auto& debugSystem = DebugSystem::getInstance();
            auto* config = debugSystem.getConfig();
            
            if (config)
            {
                // Reducir todos los buffers
                config->setInt("logger.max_history_size", 100);
                config->setInt("console.max_display_lines", 10);
                config->setInt("performance.max_history_size", 30);
                config->setInt("commands.max_history_size", 10);
                
                // Deshabilitar logging a archivo para ahorrar I/O
                config->setBool("logger.file_logging_enabled", false);
                
                config->applyToDebugSystem();
                
                INFO_LOG("Configuración de memoria limitada aplicada");
            }
        }
        
        // Verificar y reportar uso de memoria del sistema de debug
        static void reportMemoryUsage()
        {
            auto& debugSystem = DebugSystem::getInstance();
            auto* logger = debugSystem.getLogger();
            auto* console = debugSystem.getConsole();
            
            if (logger && console)
            {
                size_t logCount = logger->getLogCount();
                
                // Estimación aproximada de uso de memoria
                size_t estimatedMemory = logCount * 200; // ~200 bytes por log entry
                
                std::string report = "Uso estimado de memoria del Debug System: " + 
                                   std::to_string(estimatedMemory / 1024) + " KB (" + 
                                   std::to_string(logCount) + " logs)";
                
                console->addLine(report);
                INFO_LOG(report);
            }
        }
        
        // Limpiar datos para liberar memoria
        static void cleanupMemory()
        {
            auto& debugSystem = DebugSystem::getInstance();
            auto* logger = debugSystem.getLogger();
            auto* cmdProcessor = debugSystem.getCommandProcessor();
            
            if (logger)
            {
                size_t oldCount = logger->getLogCount();
                logger->clearHistory();
                
                INFO_LOG("Memoria limpiada: " + std::to_string(oldCount) + " logs eliminados");
            }
            
            if (cmdProcessor)
            {
                cmdProcessor->clearHistory();
                INFO_LOG("Historial de comandos limpiado");
            }
        }
    };
}