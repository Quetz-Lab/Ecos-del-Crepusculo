#pragma once
#include "DebugSystem.h"
#include "raylib.h"
#include <string>
#include <vector>
#include <deque>

namespace Quetz_LabEDC
{
    class DebugConsole
    {
    private:
        bool isVisible;
        std::deque<std::string> displayLines;
        std::string inputBuffer;
        Rectangle consoleRect;
        Color backgroundColor;
        Color textColor;
        Color inputColor;
        
        int maxDisplayLines;
        int fontSize;
        int inputCursorPos;
        bool inputActive;
        
        // Configuración visual
        float consoleHeight;
        float consoleWidth;
        float padding;
        
    public:
        DebugConsole();
        ~DebugConsole();
        
        // Control de visibilidad
        void toggle();
        void show() { isVisible = true; }
        void hide() { isVisible = false; }
        bool getIsVisible() const { return isVisible; }
        
        // Manejo de contenido
        void addLine(const std::string& line);
        void clear();
        
        // Input y comandos
        void processInput(const std::string& input);
        void handleKeyboardInput();
        
        // Renderizado y actualización
        void draw();
        void update();
        
        // Configuración
        void setMaxDisplayLines(int lines) { maxDisplayLines = lines; }
        void setFontSize(int size) { fontSize = size; }
        void setColors(Color bg, Color text, Color input);
        void setSize(float width, float height);
        
        // Estado
        bool isActive() const { return isVisible && inputActive; }
    };
}