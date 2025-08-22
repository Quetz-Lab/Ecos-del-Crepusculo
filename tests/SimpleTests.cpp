#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <algorithm>
#include <sstream>

// Simple test framework sin dependencias externas
class SimpleTestFramework
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
        
        if (totalTests > 0)
        {
            std::cout << "Porcentaje de éxito: " << (passedTests * 100.0 / totalTests) << "%" << std::endl;
        }
    }
    
    static bool allTestsPassed()
    {
        return passedTests == totalTests;
    }
};

int SimpleTestFramework::totalTests = 0;
int SimpleTestFramework::passedTests = 0;

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
        throw std::runtime_error("Assertion failed: expected != actual"); \
    }

// Simulación simple de LogLevel y LogEntry para pruebas
enum TestLogLevel
{
    TEST_LOG_DEBUG = 0,
    TEST_LOG_INFO = 1,
    TEST_LOG_WARNING = 2,
    TEST_LOG_ERROR = 3,
    TEST_LOG_CRITICAL = 4
};

struct TestLogEntry
{
    std::string message;
    TestLogLevel level;
    std::string timestamp;
    std::string file;
    int line;
    
    TestLogEntry(const std::string& msg, TestLogLevel lvl, const std::string& ts, 
                const std::string& f, int l)
        : message(msg), level(lvl), timestamp(ts), file(f), line(l) {}
};

// Logger simplificado para pruebas
class TestLogger
{
private:
    std::vector<TestLogEntry> logHistory;
    TestLogLevel minLevel;
    size_t maxHistorySize;
    
public:
    TestLogger() : minLevel(TEST_LOG_DEBUG), maxHistorySize(1000) {}
    
    void log(const std::string& message, TestLogLevel level, const char* file = "", int line = 0)
    {
        if (level < minLevel) return;
        
        logHistory.emplace_back(message, level, "test_timestamp", file ? file : "", line);
        
        if (logHistory.size() > maxHistorySize)
        {
            logHistory.erase(logHistory.begin());
        }
    }
    
    void setMinLevel(TestLogLevel level) { minLevel = level; }
    void setMaxHistorySize(size_t size) { maxHistorySize = size; }
    
    std::vector<TestLogEntry> getAllLogs() const { return logHistory; }
    size_t getLogCount() const { return logHistory.size(); }
    void clearHistory() { logHistory.clear(); }
};

// CommandProcessor simplificado para pruebas
class TestCommandProcessor
{
private:
    std::map<std::string, std::function<void(const std::vector<std::string>&)>> commands;
    
    std::string toLowerCase(const std::string& str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
    
    std::vector<std::string> parseCommand(const std::string& input)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(input);
        std::string token;
        
        while (ss >> token)
        {
            tokens.push_back(token);
        }
        
        return tokens;
    }
    
public:
    void registerCommand(const std::string& name, std::function<void(const std::vector<std::string>&)> func)
    {
        commands[toLowerCase(name)] = func;
    }
    
    bool commandExists(const std::string& name) const
    {
        return commands.find(toLowerCase(name)) != commands.end();
    }
    
    void executeCommand(const std::string& input)
    {
        auto tokens = parseCommand(input);
        if (tokens.empty()) return;
        
        std::string commandName = toLowerCase(tokens[0]);
        auto it = commands.find(commandName);
        
        if (it != commands.end())
        {
            it->second(tokens);
        }
    }
};

// Tests para TestLogger
void testLoggerBasicFunctionality()
{
    TestLogger logger;
    
    logger.log("Test message", TEST_LOG_INFO, __FILE__, __LINE__);
    
    auto logs = logger.getAllLogs();
    ASSERT_TRUE(logs.size() > 0);
    ASSERT_TRUE(logs.back().message == "Test message");
    ASSERT_TRUE(logs.back().level == TEST_LOG_INFO);
}

void testLoggerLevels()
{
    TestLogger logger;
    logger.setMinLevel(TEST_LOG_WARNING);
    
    logger.log("Debug message", TEST_LOG_DEBUG, __FILE__, __LINE__);
    logger.log("Info message", TEST_LOG_INFO, __FILE__, __LINE__);
    logger.log("Warning message", TEST_LOG_WARNING, __FILE__, __LINE__);
    logger.log("Error message", TEST_LOG_ERROR, __FILE__, __LINE__);
    
    auto logs = logger.getAllLogs();
    ASSERT_EQUAL(2, logs.size());
    ASSERT_TRUE(logs[0].level == TEST_LOG_WARNING);
    ASSERT_TRUE(logs[1].level == TEST_LOG_ERROR);
}

void testLoggerHistoryLimit()
{
    TestLogger logger;
    logger.setMaxHistorySize(3);
    
    for (int i = 0; i < 5; i++)
    {
        logger.log("Message " + std::to_string(i), TEST_LOG_INFO, __FILE__, __LINE__);
    }
    
    auto logs = logger.getAllLogs();
    ASSERT_EQUAL(3, logs.size());
    
    ASSERT_TRUE(logs[0].message == "Message 2");
    ASSERT_TRUE(logs[1].message == "Message 3");
    ASSERT_TRUE(logs[2].message == "Message 4");
}

// Tests para TestCommandProcessor
void testCommandProcessorRegistration()
{
    TestCommandProcessor processor;
    
    bool commandExecuted = false;
    processor.registerCommand("test", [&commandExecuted](const std::vector<std::string>& args) {
        commandExecuted = true;
    });
    
    ASSERT_TRUE(processor.commandExists("test"));
    
    processor.executeCommand("test");
    ASSERT_TRUE(commandExecuted);
}

void testCommandProcessorParsing()
{
    TestCommandProcessor processor;
    
    std::vector<std::string> receivedArgs;
    processor.registerCommand("testargs", [&receivedArgs](const std::vector<std::string>& args) {
        receivedArgs = args;
    });
    
    processor.executeCommand("testargs arg1 arg2 arg3");
    
    ASSERT_EQUAL(4, receivedArgs.size());
    ASSERT_TRUE(receivedArgs[0] == "testargs");
    ASSERT_TRUE(receivedArgs[1] == "arg1");
    ASSERT_TRUE(receivedArgs[2] == "arg2");
    ASSERT_TRUE(receivedArgs[3] == "arg3");
}

void testCommandProcessorCaseInsensitive()
{
    TestCommandProcessor processor;
    
    int executionCount = 0;
    processor.registerCommand("TestCase", [&executionCount](const std::vector<std::string>& args) {
        executionCount++;
    });
    
    processor.executeCommand("testcase");
    processor.executeCommand("TESTCASE");
    processor.executeCommand("TestCase");
    
    ASSERT_EQUAL(3, executionCount);
}

// Tests de configuración simulada
void testConfigurationParsing()
{
    // Simular parsing de configuración
    std::map<std::string, std::string> config;
    
    // Simular líneas de configuración
    std::vector<std::string> configLines = {
        "logger.min_level = 1",
        "console.font_size = 16",
        "debug.enabled = true",
        "# This is a comment",
        "",
        "performance.show_overlay = false"
    };
    
    for (const auto& line : configLines)
    {
        if (line.empty() || line[0] == '#') continue;
        
        size_t equalPos = line.find('=');
        if (equalPos != std::string::npos)
        {
            std::string key = line.substr(0, equalPos);
            std::string value = line.substr(equalPos + 1);
            
            // Trim spaces (simple)
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            config[key] = value;
        }
    }
    
    ASSERT_EQUAL(4, config.size());
    ASSERT_TRUE(config["logger.min_level"] == "1");
    ASSERT_TRUE(config["console.font_size"] == "16");
    ASSERT_TRUE(config["debug.enabled"] == "true");
    ASSERT_TRUE(config["performance.show_overlay"] == "false");
}

int main()
{
    std::cout << "=== Ejecutando Pruebas Simplificadas del Sistema de Debug ===" << std::endl;
    
    // Tests del Logger
    SimpleTestFramework::runTest("Logger - Funcionalidad Básica", testLoggerBasicFunctionality);
    SimpleTestFramework::runTest("Logger - Niveles de Log", testLoggerLevels);
    SimpleTestFramework::runTest("Logger - Límite de Historial", testLoggerHistoryLimit);
    
    // Tests del CommandProcessor
    SimpleTestFramework::runTest("CommandProcessor - Registro de Comandos", testCommandProcessorRegistration);
    SimpleTestFramework::runTest("CommandProcessor - Parsing de Argumentos", testCommandProcessorParsing);
    SimpleTestFramework::runTest("CommandProcessor - Case Insensitive", testCommandProcessorCaseInsensitive);
    
    // Tests de configuración
    SimpleTestFramework::runTest("Configuration - Parsing", testConfigurationParsing);
    
    SimpleTestFramework::printResults();
    
    return SimpleTestFramework::allTestsPassed() ? 0 : 1;
}