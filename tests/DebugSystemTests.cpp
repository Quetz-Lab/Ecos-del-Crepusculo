#include "../include/DebugSystem.h"
#include "../include/Logger.h"
#include "../include/CommandProcessor.h"
#include "../include/PerformanceMonitor.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>

using namespace Quetz_LabEDC;

// Simple test framework
class TestFramework
{
private:
    static int totalTests;
    static int passedTests;
    
public:
    static void runTest(const std::string& testName, std::function<void()> testFunc)
    {
        totalTests++;
        std::cout << "Ejecutando: " << testName << "... ";
        
        try
        {
            testFunc();
            passedTests++;
            std::cout << "PASÓ" << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "FALLÓ: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cout << "FALLÓ: Error desconocido" << std::endl;
        }
    }
    
    static void printResults()
    {
        std::cout << "\n=== Resultados de Pruebas ===" << std::endl;
        std::cout << "Total: " << totalTests << std::endl;
        std::cout << "Pasaron: " << passedTests << std::endl;
        std::cout << "Fallaron: " << (totalTests - passedTests) << std::endl;
        std::cout << "Porcentaje de éxito: " << (passedTests * 100.0 / totalTests) << "%" << std::endl;
    }
};

int TestFramework::totalTests = 0;
int TestFramework::passedTests = 0;

// Macro para assertions
#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed: " #condition); \
    }

#define ASSERT_FALSE(condition) \
    if (condition) { \
        throw std::runtime_error("Assertion failed: " #condition " should be false"); \
    }

#define ASSERT_EQUAL(expected, actual) \
    if ((expected) != (actual)) { \
        throw std::runtime_error("Assertion failed: expected " + std::to_string(expected) + " but got " + std::to_string(actual)); \
    }

// Tests para Logger
void testLoggerBasicFunctionality()
{
    Logger logger;
    
    // Test de logging básico
    logger.log("Test message", LOG_INFO, __FILE__, __LINE__);
    
    // Verificar que el log se guardó
    auto logs = logger.getAllLogs();
    ASSERT_TRUE(logs.size() > 0);
    ASSERT_TRUE(logs.back().message == "Test message");
    ASSERT_TRUE(logs.back().level == LOG_INFO);
}

void testLoggerLevels()
{
    Logger logger;
    
    // Configurar nivel mínimo
    logger.setMinLevel(LOG_WARNING);
    
    // Logs que no deberían aparecer
    logger.log("Debug message", LOG_DEBUG, __FILE__, __LINE__);
    logger.log("Info message", LOG_INFO, __FILE__, __LINE__);
    
    // Logs que sí deberían aparecer
    logger.log("Warning message", LOG_WARNING, __FILE__, __LINE__);
    logger.log("Error message", LOG_ERROR, __FILE__, __LINE__);
    
    auto logs = logger.getAllLogs();
    ASSERT_EQUAL(2, logs.size());
    ASSERT_TRUE(logs[0].level == LOG_WARNING);
    ASSERT_TRUE(logs[1].level == LOG_ERROR);
}

void testLoggerHistoryLimit()
{
    Logger logger;
    logger.setMaxHistorySize(3);
    
    // Agregar más logs que el límite
    for (int i = 0; i < 5; i++)
    {
        logger.log("Message " + std::to_string(i), LOG_INFO, __FILE__, __LINE__);
    }
    
    auto logs = logger.getAllLogs();
    ASSERT_EQUAL(3, logs.size());
    
    // Verificar que se mantuvieron los más recientes
    ASSERT_TRUE(logs[0].message == "Message 2");
    ASSERT_TRUE(logs[1].message == "Message 3");
    ASSERT_TRUE(logs[2].message == "Message 4");
}

// Tests para CommandProcessor
void testCommandProcessorRegistration()
{
    CommandProcessor processor;
    
    // Registrar comando de prueba
    bool commandExecuted = false;
    processor.registerCommand("test", "Test command", "test",
        [&commandExecuted](const std::vector<std::string>& args) {
            commandExecuted = true;
        });
    
    // Verificar que el comando existe
    ASSERT_TRUE(processor.commandExists("test"));
    
    // Ejecutar comando
    processor.executeCommand("test");
    ASSERT_TRUE(commandExecuted);
}

void testCommandProcessorParsing()
{
    CommandProcessor processor;
    
    std::vector<std::string> receivedArgs;
    processor.registerCommand("testargs", "Test command with args", "testargs <arg1> <arg2>",
        [&receivedArgs](const std::vector<std::string>& args) {
            receivedArgs = args;
        });
    
    // Ejecutar comando con argumentos
    processor.executeCommand("testargs arg1 arg2 arg3");
    
    ASSERT_EQUAL(4, receivedArgs.size());
    ASSERT_TRUE(receivedArgs[0] == "testargs");
    ASSERT_TRUE(receivedArgs[1] == "arg1");
    ASSERT_TRUE(receivedArgs[2] == "arg2");
    ASSERT_TRUE(receivedArgs[3] == "arg3");
}

void testCommandProcessorCaseInsensitive()
{
    CommandProcessor processor;
    
    bool commandExecuted = false;
    processor.registerCommand("TestCase", "Test case sensitivity", "TestCase",
        [&commandExecuted](const std::vector<std::string>& args) {
            commandExecuted = true;
        });
    
    // Probar diferentes casos
    processor.executeCommand("testcase");
    ASSERT_TRUE(commandExecuted);
    
    commandExecuted = false;
    processor.executeCommand("TESTCASE");
    ASSERT_TRUE(commandExecuted);
    
    commandExecuted = false;
    processor.executeCommand("TestCase");
    ASSERT_TRUE(commandExecuted);
}

// Tests para PerformanceMonitor
void testPerformanceMonitorBasic()
{
    PerformanceMonitor monitor;
    
    // Simular actualización
    monitor.update();
    
    auto perfData = monitor.getCurrentData();
    
    // Verificar que los datos son válidos
    ASSERT_TRUE(perfData.currentFPS >= 0.0f);
    ASSERT_TRUE(perfData.frameTime >= 0.0f);
}

void testPerformanceMonitorThresholds()
{
    PerformanceMonitor monitor;
    
    // Configurar thresholds
    monitor.setLowFpsThreshold(30.0f);
    monitor.setCriticalFpsThreshold(15.0f);
    
    // Los métodos de verificación deberían funcionar
    // (Nota: En un test real necesitaríamos simular FPS bajo)
    ASSERT_FALSE(monitor.isPerformanceCritical()); // Asumiendo FPS normal
}

// Tests de integración
void testDebugSystemInitialization()
{
    auto& debugSystem = DebugSystem::getInstance();
    
    // Inicializar sistema
    debugSystem.initialize();
    
    ASSERT_TRUE(debugSystem.getIsInitialized());
    ASSERT_TRUE(debugSystem.getLogger() != nullptr);
    ASSERT_TRUE(debugSystem.getConsole() != nullptr);
    ASSERT_TRUE(debugSystem.getPerformanceMonitor() != nullptr);
    ASSERT_TRUE(debugSystem.getObjectInspector() != nullptr);
    ASSERT_TRUE(debugSystem.getCommandProcessor() != nullptr);
    
    // Limpiar
    debugSystem.shutdown();
}

void testDebugSystemLogging()
{
    auto& debugSystem = DebugSystem::getInstance();
    debugSystem.initialize();
    
    // Test de macros de logging
    DEBUG_LOG("Test debug message");
    INFO_LOG("Test info message");
    WARN_LOG("Test warning message");
    ERROR_LOG("Test error message");
    
    // Verificar que los logs se guardaron
    auto* logger = debugSystem.getLogger();
    ASSERT_TRUE(logger != nullptr);
    
    auto logs = logger->getAllLogs();
    ASSERT_TRUE(logs.size() >= 4);
    
    debugSystem.shutdown();
}

int main()
{
    std::cout << "=== Ejecutando Pruebas del Sistema de Debug ===" << std::endl;
    
    // Tests del Logger
    TestFramework::runTest("Logger - Funcionalidad Básica", testLoggerBasicFunctionality);
    TestFramework::runTest("Logger - Niveles de Log", testLoggerLevels);
    TestFramework::runTest("Logger - Límite de Historial", testLoggerHistoryLimit);
    
    // Tests del CommandProcessor
    TestFramework::runTest("CommandProcessor - Registro de Comandos", testCommandProcessorRegistration);
    TestFramework::runTest("CommandProcessor - Parsing de Argumentos", testCommandProcessorParsing);
    TestFramework::runTest("CommandProcessor - Case Insensitive", testCommandProcessorCaseInsensitive);
    
    // Tests del PerformanceMonitor
    TestFramework::runTest("PerformanceMonitor - Funcionalidad Básica", testPerformanceMonitorBasic);
    TestFramework::runTest("PerformanceMonitor - Thresholds", testPerformanceMonitorThresholds);
    
    // Tests de integración
    TestFramework::runTest("DebugSystem - Inicialización", testDebugSystemInitialization);
    TestFramework::runTest("DebugSystem - Logging", testDebugSystemLogging);
    
    TestFramework::printResults();
    
    return 0;
}