#pragma once
#include "DebugSystem.h"
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace Quetz_LabEDC
{
    // Tipo de función para comandos
    using CommandFunction = std::function<void(const std::vector<std::string>&)>;
    
    // Estructura para información de comandos
    struct CommandInfo
    {
        std::string name;
        std::string description;
        std::string usage;
        CommandFunction function;
        
        CommandInfo(const std::string& n, const std::string& desc, 
                   const std::string& u, CommandFunction func)
            : name(n), description(desc), usage(u), function(func) {}
    };
    
    class CommandProcessor
    {
    private:
        std::map<std::string, CommandInfo> commands;
        std::vector<std::string> commandHistory;
        size_t maxHistorySize;
        
        // Métodos privados
        std::vector<std::string> parseCommand(const std::string& input);
        std::string toLowerCase(const std::string& str);
        void addToHistory(const std::string& command);
        
        // Comandos predeterminados
        void initializeDefaultCommands();
        void cmdHelp(const std::vector<std::string>& args);
        void cmdClear(const std::vector<std::string>& args);
        void cmdFps(const std::vector<std::string>& args);
        void cmdMemory(const std::vector<std::string>& args);
        void cmdObjects(const std::vector<std::string>& args);
        void cmdInspect(const std::vector<std::string>& args);
        void cmdTeleport(const std::vector<std::string>& args);
        void cmdSpawn(const std::vector<std::string>& args);
        void cmdSave(const std::vector<std::string>& args);
        void cmdLoad(const std::vector<std::string>& args);
        void cmdExit(const std::vector<std::string>& args);
        void cmdConfig(const std::vector<std::string>& args);
        void cmdOptimize(const std::vector<std::string>& args);
        void cmdCleanup(const std::vector<std::string>& args);
        
    public:
        CommandProcessor();
        ~CommandProcessor();
        
        // Registro de comandos
        void registerCommand(const std::string& name, const std::string& description,
                           const std::string& usage, CommandFunction function);
        void unregisterCommand(const std::string& name);
        
        // Ejecución de comandos
        void executeCommand(const std::string& input);
        bool commandExists(const std::string& name) const;
        
        // Información de comandos
        std::vector<std::string> getAvailableCommands() const;
        std::vector<CommandInfo> getAllCommandInfo() const;
        CommandInfo getCommandInfo(const std::string& name) const;
        
        // Historial
        std::vector<std::string> getCommandHistory() const { return commandHistory; }
        void clearHistory() { commandHistory.clear(); }
        void setMaxHistorySize(size_t size) { maxHistorySize = size; }
        
        // Autocompletado
        std::vector<std::string> getSuggestions(const std::string& partial) const;
        std::string getClosestMatch(const std::string& input) const;
    };
}