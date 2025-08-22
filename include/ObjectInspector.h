#pragma once
#include "DebugSystem.h"
#include "GameObject.h"
#include "raylib.h"
#include <string>
#include <vector>
#include <map>

namespace Quetz_LabEDC
{
    // Estructura para propiedades inspeccionables
    struct InspectableProperty
    {
        std::string name;
        std::string value;
        std::string type;
        bool isEditable;
        
        InspectableProperty(const std::string& n, const std::string& v, 
                          const std::string& t, bool editable = false)
            : name(n), value(v), type(t), isEditable(editable) {}
    };
    
    class ObjectInspector
    {
    private:
        GameObject* selectedObject;
        bool isActive;
        Rectangle inspectorWindow;
        
        // UI del inspector
        Vector2 windowPosition;
        Vector2 windowSize;
        Color windowColor;
        Color textColor;
        Color headerColor;
        
        int fontSize;
        float lineHeight;
        float padding;
        
        // Propiedades del objeto seleccionado
        std::vector<InspectableProperty> properties;
        int selectedPropertyIndex;
        std::string editBuffer;
        bool isEditingProperty;
        
        // Métodos privados
        void updateProperties();
        void drawProperty(const InspectableProperty& prop, int index, Vector2 position);
        void handlePropertyEdit();
        GameObject* findObjectAtPosition(Vector2 position);
        std::string formatVector2(Vector2 vec);
        Vector2 parseVector2(const std::string& str);
        
    public:
        ObjectInspector();
        ~ObjectInspector();
        
        // Control del inspector
        void selectObject(GameObject* obj);
        void selectObjectAtPosition(Vector2 position);
        void deselectObject();
        
        // Estado
        bool isInspecting() const { return isActive && selectedObject != nullptr; }
        GameObject* getSelectedObject() const { return selectedObject; }
        
        // Renderizado y actualización
        void drawInspector();
        void update();
        void handleInput();
        
        // Modificación de propiedades
        void updateObjectProperty(const std::string& property, const std::string& value);
        bool canEditProperty(const std::string& property) const;
        
        // Configuración
        void setWindowPosition(Vector2 position) { windowPosition = position; }
        void setWindowSize(Vector2 size) { windowSize = size; }
        void setColors(Color window, Color text, Color header);
        void setFontSize(int size) { fontSize = size; }
        
        // Activación/desactivación
        void activate() { isActive = true; }
        void deactivate() { isActive = false; }
        void toggle() { isActive = !isActive; }
    };
}