#include "DebugConsole.h"
#include "DebugSystem.h"
#include <algorithm>
#include <sstream>

namespace Quetz_LabEDC
{
    DebugConsole::DebugConsole()
        : isVisible(false)
        , backgroundColor({0, 0, 0, 200})
        , textColor(WHITE)
        , inputColor(YELLOW)
        , maxDisplayLines(20)
        , fontSize(16)
        , inputCursorPos(0)
        , inputActive(false)
        , consoleHeight(400.0f)
        , consoleWidth(800.0f)
        , padding(10.0f)
    {
        // Configurar rectángulo de la consola
        consoleRect = {
            padding,
            padding,
            consoleWidth,
            consoleHeight
        };
        
        // Mensaje de bienvenida
        addLine("=== Consola de Debug de Ecos del Crepusculo ===");
        addLine("Presiona F1 para mostrar/ocultar la consola");
        addLine("Escribe 'help' para ver comandos disponibles");
        addLine("");
    }
    
    DebugConsole::~DebugConsole()
    {
        // Cleanup si es necesario
    }
    
    void DebugConsole::toggle()
    {
        isVisible = !isVisible;
        
        if (isVisible)
        {
            inputActive = true;
            inputCursorPos = inputBuffer.length();
        }
        else
        {
            inputActive = false;
        }
    }
    
    void DebugConsole::addLine(const std::string& line)
    {
        displayLines.push_back(line);
        
        // Mantener solo las líneas más recientes
        while (displayLines.size() > static_cast<size_t>(maxDisplayLines))
        {
            displayLines.pop_front();
        }
    }
    
    void DebugConsole::clear()
    {
        displayLines.clear();
        addLine("Consola limpiada");
    }
    
    void DebugConsole::processInput(const std::string& input)
    {
        if (input.empty())
        {
            return;
        }
        
        // Mostrar el comando ejecutado
        addLine("> " + input);
        
        // Procesar el comando a través del CommandProcessor
        auto& debugSystem = DebugSystem::getInstance();
        auto* cmdProcessor = debugSystem.getCommandProcessor();
        
        if (cmdProcessor)
        {
            cmdProcessor->executeCommand(input);
        }
        else
        {
            addLine("Error: CommandProcessor no disponible");
        }
    }
    
    void DebugConsole::handleKeyboardInput()
    {
        if (!isVisible || !inputActive)
        {
            return;
        }
        
        // Obtener caracteres presionados
        int key = GetCharPressed();
        while (key > 0)
        {
            // Solo caracteres imprimibles
            if (key >= 32 && key <= 125)
            {
                inputBuffer.insert(inputCursorPos, 1, static_cast<char>(key));
                inputCursorPos++;
            }
            key = GetCharPressed();
        }
        
        // Manejar teclas especiales
        if (IsKeyPressed(KEY_ENTER))
        {
            processInput(inputBuffer);
            inputBuffer.clear();
            inputCursorPos = 0;
        }
        else if (IsKeyPressed(KEY_BACKSPACE) && !inputBuffer.empty() && inputCursorPos > 0)
        {
            inputBuffer.erase(inputCursorPos - 1, 1);
            inputCursorPos--;
        }
        else if (IsKeyPressed(KEY_DELETE) && inputCursorPos < inputBuffer.length())
        {
            inputBuffer.erase(inputCursorPos, 1);
        }
        else if (IsKeyPressed(KEY_LEFT) && inputCursorPos > 0)
        {
            inputCursorPos--;
        }
        else if (IsKeyPressed(KEY_RIGHT) && inputCursorPos < inputBuffer.length())
        {
            inputCursorPos++;
        }
        else if (IsKeyPressed(KEY_HOME))
        {
            inputCursorPos = 0;
        }
        else if (IsKeyPressed(KEY_END))
        {
            inputCursorPos = inputBuffer.length();
        }
    }
    
    void DebugConsole::update()
    {
        if (!isVisible)
        {
            return;
        }
        
        handleKeyboardInput();
        
        // Actualizar con nuevos logs del sistema
        auto& debugSystem = DebugSystem::getInstance();
        auto* logger = debugSystem.getLogger();
        
        if (logger)
        {
            static size_t lastLogCount = 0;
            size_t currentLogCount = logger->getLogCount();
            
            if (currentLogCount > lastLogCount)
            {
                // Hay nuevos logs, obtener los más recientes
                auto recentLogs = logger->getRecentLogs(currentLogCount - lastLogCount);
                
                for (const auto& logEntry : recentLogs)
                {
                    // Formatear el log para la consola
                    std::stringstream ss;
                    ss << "[" << logEntry.timestamp.substr(11, 8) << "] "; // Solo hora
                    
                    // Color basado en nivel
                    std::string levelStr;
                    switch (logEntry.level)
                    {
                        case LOG_DEBUG:    levelStr = "DBG"; break;
                        case LOG_INFO:     levelStr = "INF"; break;
                        case LOG_WARNING:  levelStr = "WRN"; break;
                        case LOG_ERROR:    levelStr = "ERR"; break;
                        case LOG_CRITICAL: levelStr = "CRT"; break;
                    }
                    
                    ss << "[" << levelStr << "] " << logEntry.message;
                    addLine(ss.str());
                }
                
                lastLogCount = currentLogCount;
            }
        }
    }
    
    void DebugConsole::draw()
    {
        if (!isVisible)
        {
            return;
        }
        
        // Dibujar fondo de la consola
        DrawRectangleRec(consoleRect, backgroundColor);
        DrawRectangleLinesEx(consoleRect, 2.0f, WHITE);
        
        // Calcular posiciones
        float lineHeight = fontSize + 2.0f;
        float startY = consoleRect.y + padding;
        float textX = consoleRect.x + padding;
        
        // Dibujar líneas de texto
        int lineIndex = 0;
        for (const auto& line : displayLines)
        {
            float y = startY + (lineIndex * lineHeight);
            
            // No dibujar líneas que estén fuera del área visible
            if (y + lineHeight > consoleRect.y + consoleRect.height - 40) // Reservar espacio para input
            {
                break;
            }
            
            // Determinar color basado en el contenido
            Color lineColor = textColor;
            if (line.find("[ERR]") != std::string::npos || line.find("[CRT]") != std::string::npos)
            {
                lineColor = RED;
            }
            else if (line.find("[WRN]") != std::string::npos)
            {
                lineColor = ORANGE;
            }
            else if (line.find(">") == 0) // Comandos ejecutados
            {
                lineColor = inputColor;
            }
            
            DrawText(line.c_str(), static_cast<int>(textX), static_cast<int>(y), fontSize, lineColor);
            lineIndex++;
        }
        
        // Dibujar línea de separación
        float separatorY = consoleRect.y + consoleRect.height - 35;
        DrawLine(
            static_cast<int>(consoleRect.x + padding),
            static_cast<int>(separatorY),
            static_cast<int>(consoleRect.x + consoleRect.width - padding),
            static_cast<int>(separatorY),
            WHITE
        );
        
        // Dibujar prompt de input
        float inputY = separatorY + 5;
        std::string prompt = "> " + inputBuffer;
        DrawText(prompt.c_str(), static_cast<int>(textX), static_cast<int>(inputY), fontSize, inputColor);
        
        // Dibujar cursor parpadeante
        if (inputActive && (static_cast<int>(GetTime() * 2) % 2 == 0))
        {
            int cursorX = static_cast<int>(textX + MeasureText(("> " + inputBuffer.substr(0, inputCursorPos)).c_str(), fontSize));
            DrawLine(cursorX, static_cast<int>(inputY), cursorX, static_cast<int>(inputY + fontSize), inputColor);
        }
        
        // Dibujar información de ayuda en la esquina
        std::string helpText = "F1: Toggle | ESC: Cerrar | Enter: Ejecutar";
        int helpWidth = MeasureText(helpText.c_str(), 12);
        DrawText(
            helpText.c_str(),
            static_cast<int>(consoleRect.x + consoleRect.width - helpWidth - padding),
            static_cast<int>(consoleRect.y + padding),
            12,
            LIGHTGRAY
        );
    }
    
    void DebugConsole::setColors(Color bg, Color text, Color input)
    {
        backgroundColor = bg;
        textColor = text;
        inputColor = input;
    }
    
    void DebugConsole::setSize(float width, float height)
    {
        consoleWidth = width;
        consoleHeight = height;
        
        consoleRect.width = width;
        consoleRect.height = height;
    }
}