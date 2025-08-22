#include "PerformanceMonitor.h"
#include "DebugSystem.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#include <unistd.h>
#endif

namespace Quetz_LabEDC
{
    PerformanceMonitor::PerformanceMonitor()
        : showOverlay(false)
        , lowFpsThreshold(30.0f)
        , criticalFpsThreshold(15.0f)
        , maxHistorySize(60)
        , overlayPosition({10.0f, 10.0f})
        , overlayColor(WHITE)
        , overlayFontSize(16)
    {
        lastFrameTime = std::chrono::high_resolution_clock::now();
        frameTimeHistory.reserve(maxHistorySize);
        perfData.fpsHistory.reserve(maxHistorySize);
    }
    
    PerformanceMonitor::~PerformanceMonitor()
    {
        // Cleanup si es necesario
    }
    
    void PerformanceMonitor::update()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;
        
        // Actualizar datos de rendimiento
        perfData.currentFPS = GetFPS();
        perfData.frameTime = deltaTime * 1000.0f; // Convertir a milisegundos
        perfData.memoryUsage = getCurrentMemoryUsage();
        
        // Actualizar historial
        updateFpsHistory();
        calculateAverageFps();
        
        // Verificar thresholds de rendimiento
        checkPerformanceThresholds();
    }
    
    void PerformanceMonitor::draw()
    {
        if (!showOverlay)
        {
            return;
        }
        
        // Preparar texto del overlay
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1);
        ss << "FPS: " << perfData.currentFPS << " (Avg: " << perfData.averageFPS << ")\n";
        ss << "Frame Time: " << perfData.frameTime << " ms\n";
        ss << "Memory: " << (perfData.memoryUsage / 1024 / 1024) << " MB\n";
        
        // Agregar indicador de estado
        if (isPerformanceCritical())
        {
            ss << "Status: CRITICAL";
        }
        else if (isPerformanceLow())
        {
            ss << "Status: LOW";
        }
        else
        {
            ss << "Status: OK";
        }
        
        std::string overlayText = ss.str();
        
        // Calcular tamaño del fondo
        Vector2 textSize = MeasureTextEx(GetFontDefault(), overlayText.c_str(), overlayFontSize, 1.0f);
        Rectangle backgroundRect;
        backgroundRect.x = overlayPosition.x - 5;
        backgroundRect.y = overlayPosition.y - 5;
        backgroundRect.width = textSize.x + 10;
        backgroundRect.height = textSize.y + 10;
        
        // Dibujar fondo semi-transparente
        DrawRectangleRec(backgroundRect, {0, 0, 0, 150});
        DrawRectangleLinesEx(backgroundRect, 1.0f, overlayColor);
        
        // Determinar color del texto basado en rendimiento
        Color textColor = overlayColor;
        if (isPerformanceCritical())
        {
            textColor = RED;
        }
        else if (isPerformanceLow())
        {
            textColor = ORANGE;
        }
        
        // Dibujar texto
        DrawText(
            overlayText.c_str(),
            static_cast<int>(overlayPosition.x),
            static_cast<int>(overlayPosition.y),
            overlayFontSize,
            textColor
        );
        
        // Dibujar mini gráfico de FPS si hay suficiente historial
        if (perfData.fpsHistory.size() > 10)
        {
            drawFpsGraph();
        }
    }
    
    void PerformanceMonitor::updateFpsHistory()
    {
        frameTimeHistory.push_back(perfData.frameTime);
        perfData.fpsHistory.push_back(perfData.currentFPS);
        
        // Mantener tamaño del historial
        if (frameTimeHistory.size() > maxHistorySize)
        {
            frameTimeHistory.erase(frameTimeHistory.begin());
        }
        
        if (perfData.fpsHistory.size() > maxHistorySize)
        {
            perfData.fpsHistory.erase(perfData.fpsHistory.begin());
        }
    }
    
    void PerformanceMonitor::calculateAverageFps()
    {
        if (perfData.fpsHistory.empty())
        {
            perfData.averageFPS = 0.0f;
            return;
        }
        
        float sum = 0.0f;
        for (float fps : perfData.fpsHistory)
        {
            sum += fps;
        }
        
        perfData.averageFPS = sum / perfData.fpsHistory.size();
    }
    
    size_t PerformanceMonitor::getCurrentMemoryUsage()
    {
#ifdef _WIN32
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        {
            return pmc.WorkingSetSize;
        }
#else
        struct rusage usage;
        if (getrusage(RUSAGE_SELF, &usage) == 0)
        {
            return usage.ru_maxrss * 1024; // ru_maxrss está en KB en Linux
        }
#endif
        return 0;
    }
    
    void PerformanceMonitor::checkPerformanceThresholds()
    {
        static float lastWarningTime = 0.0f;
        float currentTime = GetTime();
        
        // Evitar spam de warnings (solo cada 5 segundos)
        if (currentTime - lastWarningTime < 5.0f)
        {
            return;
        }
        
        if (isPerformanceCritical())
        {
            auto& debugSystem = DebugSystem::getInstance();
            std::stringstream ss;
            ss << "Rendimiento CRITICO detectado - FPS: " << perfData.currentFPS 
               << " (Threshold: " << criticalFpsThreshold << ")";
            debugSystem.logCritical(ss.str(), __FILE__, __LINE__);
            lastWarningTime = currentTime;
        }
        else if (isPerformanceLow())
        {
            auto& debugSystem = DebugSystem::getInstance();
            std::stringstream ss;
            ss << "Rendimiento BAJO detectado - FPS: " << perfData.currentFPS 
               << " (Threshold: " << lowFpsThreshold << ")";
            debugSystem.logWarning(ss.str(), __FILE__, __LINE__);
            lastWarningTime = currentTime;
        }
    }
    
    bool PerformanceMonitor::isPerformanceCritical() const
    {
        return perfData.currentFPS < criticalFpsThreshold;
    }
    
    bool PerformanceMonitor::isPerformanceLow() const
    {
        return perfData.currentFPS < lowFpsThreshold && perfData.currentFPS >= criticalFpsThreshold;
    }
    
    void PerformanceMonitor::setMaxHistorySize(size_t size)
    {
        maxHistorySize = size;
        frameTimeHistory.reserve(size);
        perfData.fpsHistory.reserve(size);
        
        // Recortar historial existente si es necesario
        if (frameTimeHistory.size() > size)
        {
            frameTimeHistory.erase(frameTimeHistory.begin(), 
                                 frameTimeHistory.begin() + (frameTimeHistory.size() - size));
        }
        
        if (perfData.fpsHistory.size() > size)
        {
            perfData.fpsHistory.erase(perfData.fpsHistory.begin(),
                                    perfData.fpsHistory.begin() + (perfData.fpsHistory.size() - size));
        }
    }
    
    void PerformanceMonitor::drawFpsGraph()
    {
        if (perfData.fpsHistory.size() < 2)
        {
            return;
        }
        
        // Configuración del gráfico
        float graphWidth = 200.0f;
        float graphHeight = 60.0f;
        Vector2 graphPos = {
            overlayPosition.x,
            overlayPosition.y + 80.0f // Debajo del texto principal
        };
        
        // Fondo del gráfico
        Rectangle graphRect;
        graphRect.x = graphPos.x;
        graphRect.y = graphPos.y;
        graphRect.width = graphWidth;
        graphRect.height = graphHeight;
        DrawRectangleRec(graphRect, {0, 0, 0, 100});
        DrawRectangleLinesEx(graphRect, 1.0f, WHITE);
        
        // Encontrar min/max para escalar
        float minFps = *std::min_element(perfData.fpsHistory.begin(), perfData.fpsHistory.end());
        float maxFps = *std::max_element(perfData.fpsHistory.begin(), perfData.fpsHistory.end());
        
        // Asegurar un rango mínimo
        if (maxFps - minFps < 10.0f)
        {
            maxFps = minFps + 10.0f;
        }
        
        // Dibujar líneas del gráfico
        for (size_t i = 1; i < perfData.fpsHistory.size(); ++i)
        {
            float x1 = graphPos.x + ((i - 1) * graphWidth) / (perfData.fpsHistory.size() - 1);
            float x2 = graphPos.x + (i * graphWidth) / (perfData.fpsHistory.size() - 1);
            
            float y1 = graphPos.y + graphHeight - 
                      ((perfData.fpsHistory[i - 1] - minFps) / (maxFps - minFps)) * graphHeight;
            float y2 = graphPos.y + graphHeight - 
                      ((perfData.fpsHistory[i] - minFps) / (maxFps - minFps)) * graphHeight;
            
            // Color basado en FPS
            Color lineColor = GREEN;
            if (perfData.fpsHistory[i] < criticalFpsThreshold)
            {
                lineColor = RED;
            }
            else if (perfData.fpsHistory[i] < lowFpsThreshold)
            {
                lineColor = ORANGE;
            }
            
            DrawLine(static_cast<int>(x1), static_cast<int>(y1), 
                    static_cast<int>(x2), static_cast<int>(y2), lineColor);
        }
        
        // Etiquetas del gráfico
        DrawText(TextFormat("%.0f", maxFps), static_cast<int>(graphPos.x + graphWidth + 5), 
                static_cast<int>(graphPos.y), 10, WHITE);
        DrawText(TextFormat("%.0f", minFps), static_cast<int>(graphPos.x + graphWidth + 5), 
                static_cast<int>(graphPos.y + graphHeight - 10), 10, WHITE);
    }
}