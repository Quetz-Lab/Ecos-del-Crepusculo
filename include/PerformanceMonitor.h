#pragma once
#include "DebugSystem.h"
#include "raylib.h"
#include <vector>
#include <chrono>

namespace Quetz_LabEDC
{
    class PerformanceMonitor
    {
    private:
        PerformanceData perfData;
        bool showOverlay;
        
        // Configuración de thresholds
        float lowFpsThreshold;
        float criticalFpsThreshold;
        
        // Tracking de tiempo
        std::chrono::high_resolution_clock::time_point lastFrameTime;
        std::vector<float> frameTimeHistory;
        size_t maxHistorySize;
        
        // Posición del overlay
        Vector2 overlayPosition;
        Color overlayColor;
        int overlayFontSize;
        
        // Métodos privados
        void updateFpsHistory();
        void calculateAverageFps();
        size_t getCurrentMemoryUsage();
        void drawFpsGraph();
        
    public:
        PerformanceMonitor();
        ~PerformanceMonitor();
        
        // Actualización y renderizado
        void update();
        void draw();
        
        // Control del overlay
        void toggleOverlay() { showOverlay = !showOverlay; }
        void setOverlayVisible(bool visible) { showOverlay = visible; }
        bool isOverlayVisible() const { return showOverlay; }
        
        // Acceso a datos
        PerformanceData getCurrentData() const { return perfData; }
        float getCurrentFPS() const { return perfData.currentFPS; }
        float getAverageFPS() const { return perfData.averageFPS; }
        float getFrameTime() const { return perfData.frameTime; }
        size_t getMemoryUsage() const { return perfData.memoryUsage; }
        
        // Configuración
        void setLowFpsThreshold(float threshold) { lowFpsThreshold = threshold; }
        void setCriticalFpsThreshold(float threshold) { criticalFpsThreshold = threshold; }
        void setOverlayPosition(Vector2 position) { overlayPosition = position; }
        void setOverlayColor(Color color) { overlayColor = color; }
        void setMaxHistorySize(size_t size);
        
        // Detección de problemas
        void checkPerformanceThresholds();
        bool isPerformanceCritical() const;
        bool isPerformanceLow() const;
    };
}