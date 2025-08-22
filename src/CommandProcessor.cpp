#include "CommandProcessor.h"
#include "DebugSystem.h"
#include "DebugOptimizations.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "sideKick.h"
#include <algorithm>
#include <sstream>
#include <cctype>

namespace Quetz_LabEDC
{
    CommandProcessor::CommandProcessor()
        : maxHistorySize(50)
    {
        initializeDefaultCommands();
    }
    
    CommandProcessor::~CommandProcessor()
    {
        // Cleanup si es necesario
    }
    
    void CommandProcessor::registerCommand(const std::string& name, const std::string& description,
                                         const std::string& usage, CommandFunction function)
    {
        std::string lowerName = toLowerCase(name);
        commands[lowerName] = CommandInfo(name, description, usage, function);
        
        auto& debugSystem = DebugSystem::getInstance();
        debugSystem.logDebug("Comando registrado: " + name, __FILE__, __LINE__);
    }
    
    void CommandProcessor::unregisterCommand(const std::string& name)
    {
        std::string lowerName = toLowerCase(name);
        auto it = commands.find(lowerName);
        if (it != commands.end())
        {
            commands.erase(it);
            
            auto& debugSystem = DebugSystem::getInstance();
            debugSystem.logDebug("Comando desregistrado: " + name, __FILE__, __LINE__);
        }
    }
    
    void CommandProcessor::executeCommand(const std::string& input)
    {
        if (input.empty())
        {
            return;
        }
        
        addToHistory(input);
        
        std::vector<std::string> tokens = parseCommand(input);
        if (tokens.empty())
        {
            return;
        }
        
        std::string commandName = toLowerCase(tokens[0]);
        auto it = commands.find(commandName);
        
        if (it != commands.end())
        {
            try
            {
                // Ejecutar el comando
                it->second.function(tokens);
            }
            catch (const std::exception& e)
            {
                auto& debugSystem = DebugSystem::getInstance();
                debugSystem.logError("Error ejecutando comando '" + commandName + "': " + e.what(), __FILE__, __LINE__);
                
                // Mostrar mensaje en consola
                auto* console = debugSystem.getConsole();
                if (console)
                {
                    console->addLine("Error: " + std::string(e.what()));
                }
            }
        }
        else
        {
            auto& debugSystem = DebugSystem::getInstance();
            auto* console = debugSystem.getConsole();
            
            if (console)
            {
                console->addLine("Comando desconocido: " + commandName);
                
                // Sugerir comando similar
                std::string suggestion = getClosestMatch(commandName);
                if (!suggestion.empty())
                {
                    console->addLine("¿Quisiste decir '" + suggestion + "'?");
                }
                
                console->addLine("Escribe 'help' para ver comandos disponibles");
            }
        }
    }
    
    std::vector<std::string> CommandProcessor::parseCommand(const std::string& input)
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
    
    std::string CommandProcessor::toLowerCase(const std::string& str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
    
    void CommandProcessor::addToHistory(const std::string& command)
    {
        commandHistory.push_back(command);
        
        if (commandHistory.size() > maxHistorySize)
        {
            commandHistory.erase(commandHistory.begin());
        }
    }
    
    void CommandProcessor::initializeDefaultCommands()
    {
        // Comando help
        registerCommand("help", "Muestra la lista de comandos disponibles", "help [comando]",
            [this](const std::vector<std::string>& args) { cmdHelp(args); });
        
        // Comando clear
        registerCommand("clear", "Limpia la consola de debug", "clear",
            [this](const std::vector<std::string>& args) { cmdClear(args); });
        
        // Comando fps
        registerCommand("fps", "Muestra información de rendimiento", "fps",
            [this](const std::vector<std::string>& args) { cmdFps(args); });
        
        // Comando memory
        registerCommand("memory", "Muestra información de memoria", "memory",
            [this](const std::vector<std::string>& args) { cmdMemory(args); });
        
        // Comando objects
        registerCommand("objects", "Lista todos los GameObjects", "objects",
            [this](const std::vector<std::string>& args) { cmdObjects(args); });
        
        // Comando inspect
        registerCommand("inspect", "Inspecciona un objeto específico", "inspect <nombre>",
            [this](const std::vector<std::string>& args) { cmdInspect(args); });
        
        // Comando teleport
        registerCommand("teleport", "Teletransporta al jugador", "teleport <x> <y>",
            [this](const std::vector<std::string>& args) { cmdTeleport(args); });
        
        // Comando spawn
        registerCommand("spawn", "Crea un nuevo objeto", "spawn <tipo> [x] [y]",
            [this](const std::vector<std::string>& args) { cmdSpawn(args); });
        
        // Comando save
        registerCommand("save", "Guarda el estado actual", "save",
            [this](const std::vector<std::string>& args) { cmdSave(args); });
        
        // Comando load
        registerCommand("load", "Carga un estado guardado", "load",
            [this](const std::vector<std::string>& args) { cmdLoad(args); });
        
        // Comando exit
        registerCommand("exit", "Cierra la consola de debug", "exit",
            [this](const std::vector<std::string>& args) { cmdExit(args); });
        
        // Comando config
        registerCommand("config", "Muestra o modifica configuración", "config [clave] [valor]",
            [this](const std::vector<std::string>& args) { cmdConfig(args); });
        
        // Comandos de optimización
        registerCommand("optimize", "Optimiza el sistema de debug", "optimize [auto|dev|test|release|perf|memory]",
            [this](const std::vector<std::string>& args) { cmdOptimize(args); });
        
        registerCommand("cleanup", "Limpia memoria del sistema de debug", "cleanup",
            [this](const std::vector<std::string>& args) { cmdCleanup(args); });
    }
    
    void CommandProcessor::cmdHelp(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        
        if (!console) return;
        
        if (args.size() > 1)
        {
            // Ayuda para comando específico
            std::string commandName = toLowerCase(args[1]);
            auto it = commands.find(commandName);
            
            if (it != commands.end())
            {
                const auto& cmd = it->second;
                console->addLine("Comando: " + cmd.name);
                console->addLine("Descripción: " + cmd.description);
                console->addLine("Uso: " + cmd.usage);
            }
            else
            {
                console->addLine("Comando no encontrado: " + args[1]);
            }
        }
        else
        {
            // Lista todos los comandos
            console->addLine("=== Comandos Disponibles ===");
            
            for (const auto& pair : commands)
            {
                const auto& cmd = pair.second;
                console->addLine(cmd.name + " - " + cmd.description);
            }
            
            console->addLine("");
            console->addLine("Usa 'help <comando>' para más información");
        }
    }
    
    void CommandProcessor::cmdClear(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        
        if (console)
        {
            console->clear();
        }
    }
    
    void CommandProcessor::cmdFps(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        auto* perfMonitor = debugSystem.getPerformanceMonitor();
        
        if (!console || !perfMonitor) return;
        
        auto perfData = perfMonitor->getCurrentData();
        
        console->addLine("=== Información de Rendimiento ===");
        console->addLine("FPS Actual: " + std::to_string(static_cast<int>(perfData.currentFPS)));
        console->addLine("FPS Promedio: " + std::to_string(static_cast<int>(perfData.averageFPS)));
        console->addLine("Tiempo de Frame: " + std::to_string(perfData.frameTime) + " ms");
        
        if (perfMonitor->isPerformanceCritical())
        {
            console->addLine("Estado: CRÍTICO");
        }
        else if (perfMonitor->isPerformanceLow())
        {
            console->addLine("Estado: BAJO");
        }
        else
        {
            console->addLine("Estado: OK");
        }
    }
    
    void CommandProcessor::cmdMemory(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        auto* perfMonitor = debugSystem.getPerformanceMonitor();
        
        if (!console || !perfMonitor) return;
        
        size_t memoryUsage = perfMonitor->getMemoryUsage();
        
        console->addLine("=== Información de Memoria ===");
        console->addLine("Uso actual: " + std::to_string(memoryUsage / 1024 / 1024) + " MB");
        console->addLine("GameObjects en memoria: " + std::to_string(GameObject::gameObjects.size()));
    }
    
    void CommandProcessor::cmdObjects(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        
        if (!console) return;
        
        console->addLine("=== Lista de GameObjects ===");
        
        if (GameObject::gameObjects.empty())
        {
            console->addLine("No hay objetos en la escena");
            return;
        }
        
        for (size_t i = 0; i < GameObject::gameObjects.size(); ++i)
        {
            GameObject* obj = GameObject::gameObjects[i];
            if (!obj) continue;
            
            std::string type = "GameObject";
            if (dynamic_cast<Player*>(obj)) type = "Player";
            else if (dynamic_cast<Enemy*>(obj)) type = "Enemy";
            else if (dynamic_cast<sideKick*>(obj)) type = "SideKick";
            
            std::stringstream ss;
            ss << i << ": " << obj->name << " (" << type << ") ";
            ss << "Pos: (" << static_cast<int>(obj->position.x) << "," << static_cast<int>(obj->position.y) << ")";
            
            console->addLine(ss.str());
        }
    }
    
    void CommandProcessor::cmdInspect(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        auto* inspector = debugSystem.getObjectInspector();
        
        if (!console || !inspector) return;
        
        if (args.size() < 2)
        {
            console->addLine("Uso: inspect <nombre>");
            return;
        }
        
        std::string objectName = args[1];
        GameObject* foundObject = nullptr;
        
        // Buscar objeto por nombre
        for (GameObject* obj : GameObject::gameObjects)
        {
            if (obj && obj->name == objectName)
            {
                foundObject = obj;
                break;
            }
        }
        
        if (foundObject)
        {
            inspector->selectObject(foundObject);
            inspector->activate();
            console->addLine("Inspeccionando objeto: " + objectName);
        }
        else
        {
            console->addLine("Objeto no encontrado: " + objectName);
            console->addLine("Usa 'objects' para ver la lista de objetos disponibles");
        }
    }
    
    void CommandProcessor::cmdTeleport(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        
        if (!console) return;
        
        if (args.size() < 3)
        {
            console->addLine("Uso: teleport <x> <y>");
            return;
        }
        
        try
        {
            float x = std::stof(args[1]);
            float y = std::stof(args[2]);
            
            // Buscar al jugador
            Player* player = nullptr;
            for (GameObject* obj : GameObject::gameObjects)
            {
                player = dynamic_cast<Player*>(obj);
                if (player) break;
            }
            
            if (player)
            {
                player->position = {x, y};
                console->addLine("Jugador teletransportado a (" + std::to_string(static_cast<int>(x)) + 
                               ", " + std::to_string(static_cast<int>(y)) + ")");
            }
            else
            {
                console->addLine("No se encontró al jugador en la escena");
            }
        }
        catch (const std::exception& e)
        {
            console->addLine("Error: Coordenadas inválidas");
        }
    }
    
    void CommandProcessor::cmdSpawn(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        
        if (!console) return;
        
        if (args.size() < 2)
        {
            console->addLine("Uso: spawn <tipo> [x] [y]");
            console->addLine("Tipos disponibles: enemy, sidekick");
            return;
        }
        
        std::string type = toLowerCase(args[1]);
        float x = 400.0f; // Posición por defecto
        float y = 300.0f;
        
        // Parsear coordenadas si se proporcionan
        if (args.size() >= 4)
        {
            try
            {
                x = std::stof(args[2]);
                y = std::stof(args[3]);
            }
            catch (const std::exception&)
            {
                console->addLine("Advertencia: Coordenadas inválidas, usando posición por defecto");
            }
        }
        
        // Buscar al jugador para referencia
        Player* player = nullptr;
        for (GameObject* obj : GameObject::gameObjects)
        {
            player = dynamic_cast<Player*>(obj);
            if (player) break;
        }
        
        if (type == "enemy")
        {
            if (player)
            {
                Enemy* newEnemy = new Enemy({x, y}, "SpawnedEnemy", player);
                GameObject::gameObjects.push_back(newEnemy);
                console->addLine("Enemigo creado en (" + std::to_string(static_cast<int>(x)) + 
                               ", " + std::to_string(static_cast<int>(y)) + ")");
            }
            else
            {
                console->addLine("Error: No se puede crear enemigo sin jugador en la escena");
            }
        }
        else if (type == "sidekick")
        {
            sideKick* newSidekick = new sideKick({x, y}, "SpawnedSidekick", LoadTexture("sidekick.png"));
            if (player)
            {
                newSidekick->owner = player;
            }
            GameObject::gameObjects.push_back(newSidekick);
            console->addLine("Sidekick creado en (" + std::to_string(static_cast<int>(x)) + 
                           ", " + std::to_string(static_cast<int>(y)) + ")");
        }
        else
        {
            console->addLine("Tipo de objeto desconocido: " + type);
            console->addLine("Tipos disponibles: enemy, sidekick");
        }
    }
    
    void CommandProcessor::cmdSave(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        
        if (!console) return;
        
        // Placeholder para funcionalidad de guardado
        console->addLine("Funcionalidad de guardado no implementada aún");
        console->addLine("Esta característica se agregará en futuras versiones");
    }
    
    void CommandProcessor::cmdLoad(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        
        if (!console) return;
        
        // Placeholder para funcionalidad de carga
        console->addLine("Funcionalidad de carga no implementada aún");
        console->addLine("Esta característica se agregará en futuras versiones");
    }
    
    void CommandProcessor::cmdExit(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        
        if (console)
        {
            console->hide();
        }
    }
    
    void CommandProcessor::cmdConfig(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        auto* config = debugSystem.getConfig();
        
        if (!console || !config) return;
        
        if (args.size() == 1)
        {
            // Mostrar toda la configuración
            console->addLine("=== Configuración Actual ===");
            auto allConfig = config->getAllConfig();
            
            for (const auto& pair : allConfig)
            {
                console->addLine(pair.first + " = " + pair.second);
            }
        }
        else if (args.size() == 2)
        {
            // Mostrar valor específico
            std::string key = args[1];
            std::string value = config->getString(key, "NO_ENCONTRADO");
            
            if (value == "NO_ENCONTRADO")
            {
                console->addLine("Clave de configuración no encontrada: " + key);
            }
            else
            {
                console->addLine(key + " = " + value);
            }
        }
        else if (args.size() == 3)
        {
            // Modificar valor
            std::string key = args[1];
            std::string value = args[2];
            
            config->setString(key, value);
            config->applyToDebugSystem();
            
            console->addLine("Configuración actualizada: " + key + " = " + value);
            console->addLine("Usa 'config save' para guardar los cambios");
        }
        else if (args.size() == 2 && args[1] == "save")
        {
            // Guardar configuración
            if (config->saveToFile())
            {
                console->addLine("Configuración guardada exitosamente");
            }
            else
            {
                console->addLine("Error guardando configuración");
            }
        }
        else if (args.size() == 2 && args[1] == "reload")
        {
            // Recargar configuración
            if (config->loadFromFile())
            {
                config->applyToDebugSystem();
                console->addLine("Configuración recargada exitosamente");
            }
            else
            {
                console->addLine("Error recargando configuración");
            }
        }
        else if (args.size() == 2 && args[1] == "reset")
        {
            // Resetear a valores por defecto
            config->resetToDefaults();
            config->applyToDebugSystem();
            console->addLine("Configuración restablecida a valores por defecto");
        }
        else
        {
            console->addLine("Uso: config [clave] [valor]");
            console->addLine("     config save - Guardar configuración");
            console->addLine("     config reload - Recargar configuración");
            console->addLine("     config reset - Restablecer valores por defecto");
        }
    }
    
    bool CommandProcessor::commandExists(const std::string& name) const
    {
        return commands.find(toLowerCase(name)) != commands.end();
    }
    
    std::vector<std::string> CommandProcessor::getAvailableCommands() const
    {
        std::vector<std::string> result;
        for (const auto& pair : commands)
        {
            result.push_back(pair.second.name);
        }
        return result;
    }
    
    std::vector<CommandInfo> CommandProcessor::getAllCommandInfo() const
    {
        std::vector<CommandInfo> result;
        for (const auto& pair : commands)
        {
            result.push_back(pair.second);
        }
        return result;
    }
    
    CommandInfo CommandProcessor::getCommandInfo(const std::string& name) const
    {
        auto it = commands.find(toLowerCase(name));
        if (it != commands.end())
        {
            return it->second;
        }
        
        // Retornar comando vacío si no se encuentra
        return CommandInfo("", "", "", nullptr);
    }
    
    std::vector<std::string> CommandProcessor::getSuggestions(const std::string& partial) const
    {
        std::vector<std::string> suggestions;
        std::string lowerPartial = toLowerCase(partial);
        
        for (const auto& pair : commands)
        {
            if (pair.first.find(lowerPartial) == 0) // Comienza con el texto parcial
            {
                suggestions.push_back(pair.second.name);
            }
        }
        
        return suggestions;
    }
    
    std::string CommandProcessor::getClosestMatch(const std::string& input) const
    {
        std::string lowerInput = toLowerCase(input);
        std::string bestMatch;
        int bestDistance = INT_MAX;
        
        for (const auto& pair : commands)
        {
            // Calcular distancia de Levenshtein simple
            int distance = 0;
            const std::string& cmd = pair.first;
            
            if (cmd.length() == lowerInput.length())
            {
                for (size_t i = 0; i < cmd.length(); ++i)
                {
                    if (cmd[i] != lowerInput[i])
                    {
                        distance++;
                    }
                }
            }
            else
            {
                distance = abs(static_cast<int>(cmd.length()) - static_cast<int>(lowerInput.length()));
            }
            
            if (distance < bestDistance && distance <= 2) // Solo sugerir si la distancia es pequeña
            {
                bestDistance = distance;
                bestMatch = pair.second.name;
            }
        }
        
        return bestMatch;
    }
}    
    v
oid CommandProcessor::cmdOptimize(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        
        if (!console) return;
        
        if (args.size() < 2)
        {
            console->addLine("Uso: optimize [auto|dev|test|release|perf|memory]");
            console->addLine("  auto   - Optimización automática basada en rendimiento");
            console->addLine("  dev    - Configuración para desarrollo");
            console->addLine("  test   - Configuración para testing/QA");
            console->addLine("  release- Configuración para release/producción");
            console->addLine("  perf   - Configuración para debugging de rendimiento");
            console->addLine("  memory - Configuración para memoria limitada");
            return;
        }
        
        std::string mode = args[1];
        
        if (mode == "auto")
        {
            DebugOptimizations::autoOptimize();
            console->addLine("Optimización automática aplicada");
        }
        else if (mode == "dev")
        {
            DebugOptimizations::configureForDevelopment();
            console->addLine("Configuración de desarrollo aplicada");
        }
        else if (mode == "test")
        {
            DebugOptimizations::configureForTesting();
            console->addLine("Configuración de testing aplicada");
        }
        else if (mode == "release")
        {
            DebugOptimizations::configureForRelease();
            console->addLine("Configuración de release aplicada");
        }
        else if (mode == "perf")
        {
            DebugOptimizations::configureForPerformanceDebugging();
            console->addLine("Configuración de debugging de rendimiento aplicada");
        }
        else if (mode == "memory")
        {
            DebugOptimizations::configureForLowMemory();
            console->addLine("Configuración de memoria limitada aplicada");
        }
        else
        {
            console->addLine("Modo de optimización desconocido: " + mode);
        }
    }
    
    void CommandProcessor::cmdCleanup(const std::vector<std::string>& args)
    {
        auto& debugSystem = DebugSystem::getInstance();
        auto* console = debugSystem.getConsole();
        
        if (!console) return;
        
        // Reportar uso antes de limpiar
        DebugOptimizations::reportMemoryUsage();
        
        // Limpiar memoria
        DebugOptimizations::cleanupMemory();
        
        console->addLine("Limpieza de memoria completada");
    }