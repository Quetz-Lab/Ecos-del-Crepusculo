#include "ObjectInspector.h"
#include "DebugSystem.h"
#include "Player.h"
#include "Enemy.h"
#include "sideKick.h"
#include "Weapon.h"
#include <sstream>
#include <iomanip>

namespace Quetz_LabEDC
{
    ObjectInspector::ObjectInspector()
        : selectedObject(nullptr)
        , isActive(false)
        , windowPosition({820.0f, 10.0f})
        , windowSize({300.0f, 400.0f})
        , windowColor({0, 0, 0, 200})
        , textColor(WHITE)
        , headerColor(YELLOW)
        , fontSize(14)
        , lineHeight(18.0f)
        , padding(10.0f)
        , selectedPropertyIndex(-1)
        , isEditingProperty(false)
    {
        inspectorWindow = {
            windowPosition.x,
            windowPosition.y,
            windowSize.x,
            windowSize.y
        };
    }
    
    ObjectInspector::~ObjectInspector()
    {
        // Cleanup si es necesario
    }
    
    void ObjectInspector::selectObject(GameObject* obj)
    {
        selectedObject = obj;
        selectedPropertyIndex = -1;
        isEditingProperty = false;
        editBuffer.clear();
        
        if (selectedObject)
        {
            updateProperties();
            
            auto& debugSystem = DebugSystem::getInstance();
            debugSystem.logInfo("Objeto seleccionado para inspección: " + selectedObject->name, __FILE__, __LINE__);
        }
    }
    
    void ObjectInspector::selectObjectAtPosition(Vector2 position)
    {
        GameObject* foundObject = findObjectAtPosition(position);
        selectObject(foundObject);
    }
    
    void ObjectInspector::deselectObject()
    {
        selectedObject = nullptr;
        properties.clear();
        selectedPropertyIndex = -1;
        isEditingProperty = false;
        editBuffer.clear();
    }
    
    void ObjectInspector::updateProperties()
    {
        properties.clear();
        
        if (!selectedObject)
        {
            return;
        }
        
        // Propiedades básicas de GameObject
        properties.emplace_back("Name", selectedObject->name, "string", true);
        properties.emplace_back("Position", formatVector2(selectedObject->position), "Vector2", true);
        properties.emplace_back("Display Name", selectedObject->DisplayName ? "true" : "false", "bool", true);
        
        // Información de textura
        if (selectedObject->texture.id > 0)
        {
            properties.emplace_back("Texture ID", std::to_string(selectedObject->texture.id), "int", false);
            properties.emplace_back("Texture Size", 
                                  std::to_string(selectedObject->texture.width) + "x" + 
                                  std::to_string(selectedObject->texture.height), "Vector2", false);
        }
        else
        {
            properties.emplace_back("Texture", "None", "string", false);
        }
        
        // Propiedades específicas según el tipo de objeto
        Player* player = dynamic_cast<Player*>(selectedObject);
        if (player)
        {
            properties.emplace_back("Type", "Player", "string", false);
            properties.emplace_back("Speed", std::to_string(player->speed), "float", true);
        }
        
        Enemy* enemy = dynamic_cast<Enemy*>(selectedObject);
        if (enemy)
        {
            properties.emplace_back("Type", "Enemy", "string", false);
            // Agregar propiedades específicas del enemigo si las hay
        }
        
        sideKick* sidekick = dynamic_cast<sideKick*>(selectedObject);
        if (sidekick)
        {
            properties.emplace_back("Type", "SideKick", "string", false);
            properties.emplace_back("Speed", std::to_string(sidekick->speed), "float", true);
            
            if (sidekick->owner)
            {
                properties.emplace_back("Owner", sidekick->owner->name, "string", false);
            }
            else
            {
                properties.emplace_back("Owner", "None", "string", false);
            }
        }
        
        Weapon* weapon = dynamic_cast<Weapon*>(selectedObject);
        if (weapon)
        {
            properties.emplace_back("Type", "Weapon", "string", false);
        }
    }
    
    void ObjectInspector::drawInspector()
    {
        if (!isActive)
        {
            return;
        }
        
        // Dibujar fondo de la ventana
        DrawRectangleRec(inspectorWindow, windowColor);
        DrawRectangleLinesEx(inspectorWindow, 2.0f, WHITE);
        
        float currentY = windowPosition.y + padding;
        float textX = windowPosition.x + padding;
        
        // Título del inspector
        std::string title = selectedObject ? ("Inspector: " + selectedObject->name) : "Object Inspector";
        DrawText(title.c_str(), static_cast<int>(textX), static_cast<int>(currentY), fontSize + 2, headerColor);
        currentY += lineHeight + 5;
        
        if (!selectedObject)
        {
            DrawText("Haz clic en un objeto para inspeccionarlo", 
                    static_cast<int>(textX), static_cast<int>(currentY), fontSize - 2, LIGHTGRAY);
            DrawText("o usa el comando 'inspect <nombre>'", 
                    static_cast<int>(textX), static_cast<int>(currentY + lineHeight), fontSize - 2, LIGHTGRAY);
            return;
        }
        
        // Dibujar línea separadora
        DrawLine(static_cast<int>(textX), static_cast<int>(currentY), 
                static_cast<int>(textX + windowSize.x - 2 * padding), static_cast<int>(currentY), WHITE);
        currentY += 5;
        
        // Dibujar propiedades
        for (size_t i = 0; i < properties.size(); ++i)
        {
            drawProperty(properties[i], static_cast<int>(i), {textX, currentY});
            currentY += lineHeight;
            
            // No dibujar fuera de la ventana
            if (currentY > windowPosition.y + windowSize.y - padding)
            {
                break;
            }
        }
        
        // Instrucciones de uso
        float instructionsY = windowPosition.y + windowSize.y - 60;
        DrawText("F2: Toggle Inspector", static_cast<int>(textX), static_cast<int>(instructionsY), 10, LIGHTGRAY);
        DrawText("Click: Seleccionar objeto", static_cast<int>(textX), static_cast<int>(instructionsY + 12), 10, LIGHTGRAY);
        DrawText("Enter: Editar propiedad", static_cast<int>(textX), static_cast<int>(instructionsY + 24), 10, LIGHTGRAY);
    }
    
    void ObjectInspector::drawProperty(const InspectableProperty& prop, int index, Vector2 position)
    {
        Color nameColor = textColor;
        Color valueColor = prop.isEditable ? GREEN : LIGHTGRAY;
        
        // Resaltar propiedad seleccionada
        if (index == selectedPropertyIndex)
        {
            Rectangle highlightRect = {
                position.x - 2,
                position.y - 1,
                windowSize.x - 2 * padding + 4,
                lineHeight
            };
            DrawRectangleRec(highlightRect, {100, 100, 100, 100});
        }
        
        // Dibujar nombre de la propiedad
        std::string displayName = prop.name + ":";
        DrawText(displayName.c_str(), static_cast<int>(position.x), static_cast<int>(position.y), fontSize, nameColor);
        
        // Calcular posición del valor
        int nameWidth = MeasureText(displayName.c_str(), fontSize);
        float valueX = position.x + nameWidth + 10;
        
        // Dibujar valor (o campo de edición)
        if (isEditingProperty && index == selectedPropertyIndex)
        {
            // Mostrar campo de edición
            std::string editText = editBuffer + "_"; // Cursor simple
            DrawText(editText.c_str(), static_cast<int>(valueX), static_cast<int>(position.y), fontSize, YELLOW);
        }
        else
        {
            // Mostrar valor normal
            DrawText(prop.value.c_str(), static_cast<int>(valueX), static_cast<int>(position.y), fontSize, valueColor);
        }
        
        // Indicador de editabilidad
        if (prop.isEditable)
        {
            DrawText("*", static_cast<int>(position.x + windowSize.x - 2 * padding - 10), 
                    static_cast<int>(position.y), fontSize, GREEN);
        }
    }
    
    void ObjectInspector::update()
    {
        if (!isActive)
        {
            return;
        }
        
        handleInput();
        
        // Actualizar propiedades si hay un objeto seleccionado
        if (selectedObject)
        {
            updateProperties();
        }
    }
    
    void ObjectInspector::handleInput()
    {
        if (!isActive || !selectedObject)
        {
            return;
        }
        
        // Manejar clic del mouse para seleccionar objetos
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetMousePosition();
            
            // Verificar si el clic fue dentro de la ventana del inspector
            if (CheckCollisionPointRec(mousePos, inspectorWindow))
            {
                // Calcular qué propiedad fue clickeada
                float relativeY = mousePos.y - (windowPosition.y + padding + lineHeight + 10); // Ajustar por título y separador
                int propertyIndex = static_cast<int>(relativeY / lineHeight);
                
                if (propertyIndex >= 0 && propertyIndex < static_cast<int>(properties.size()))
                {
                    selectedPropertyIndex = propertyIndex;
                }
            }
            else
            {
                // Clic fuera del inspector, seleccionar objeto en esa posición
                selectObjectAtPosition(mousePos);
            }
        }
        
        // Manejar entrada de teclado para edición
        if (selectedPropertyIndex >= 0 && selectedPropertyIndex < static_cast<int>(properties.size()))
        {
            const auto& prop = properties[selectedPropertyIndex];
            
            if (IsKeyPressed(KEY_ENTER) && prop.isEditable)
            {
                if (!isEditingProperty)
                {
                    // Comenzar edición
                    isEditingProperty = true;
                    editBuffer = prop.value;
                }
                else
                {
                    // Finalizar edición
                    updateObjectProperty(prop.name, editBuffer);
                    isEditingProperty = false;
                    editBuffer.clear();
                }
            }
            else if (IsKeyPressed(KEY_ESCAPE))
            {
                // Cancelar edición
                isEditingProperty = false;
                editBuffer.clear();
                selectedPropertyIndex = -1;
            }
        }
        
        // Manejar entrada de texto durante la edición
        if (isEditingProperty)
        {
            handlePropertyEdit();
        }
        
        // Navegación con teclas
        if (IsKeyPressed(KEY_UP) && selectedPropertyIndex > 0)
        {
            selectedPropertyIndex--;
        }
        else if (IsKeyPressed(KEY_DOWN) && selectedPropertyIndex < static_cast<int>(properties.size()) - 1)
        {
            selectedPropertyIndex++;
        }
    }
    
    void ObjectInspector::handlePropertyEdit()
    {
        // Obtener caracteres presionados
        int key = GetCharPressed();
        while (key > 0)
        {
            if (key >= 32 && key <= 125) // Caracteres imprimibles
            {
                editBuffer += static_cast<char>(key);
            }
            key = GetCharPressed();
        }
        
        // Manejar backspace
        if (IsKeyPressed(KEY_BACKSPACE) && !editBuffer.empty())
        {
            editBuffer.pop_back();
        }
    }
    
    void ObjectInspector::updateObjectProperty(const std::string& property, const std::string& value)
    {
        if (!selectedObject)
        {
            return;
        }
        
        auto& debugSystem = DebugSystem::getInstance();
        
        try
        {
            if (property == "Name")
            {
                selectedObject->name = value;
                debugSystem.logInfo("Nombre del objeto cambiado a: " + value, __FILE__, __LINE__);
            }
            else if (property == "Position")
            {
                Vector2 newPos = parseVector2(value);
                selectedObject->position = newPos;
                debugSystem.logInfo("Posición del objeto cambiada a: " + value, __FILE__, __LINE__);
            }
            else if (property == "Display Name")
            {
                selectedObject->DisplayName = (value == "true" || value == "1");
                debugSystem.logInfo("Display Name cambiado a: " + value, __FILE__, __LINE__);
            }
            else if (property == "Speed")
            {
                float speed = std::stof(value);
                
                Player* player = dynamic_cast<Player*>(selectedObject);
                if (player)
                {
                    player->speed = speed;
                    debugSystem.logInfo("Velocidad del jugador cambiada a: " + value, __FILE__, __LINE__);
                }
                
                sideKick* sidekick = dynamic_cast<sideKick*>(selectedObject);
                if (sidekick)
                {
                    sidekick->speed = speed;
                    debugSystem.logInfo("Velocidad del sidekick cambiada a: " + value, __FILE__, __LINE__);
                }
            }
        }
        catch (const std::exception& e)
        {
            debugSystem.logError("Error actualizando propiedad " + property + ": " + e.what(), __FILE__, __LINE__);
        }
    }
    
    GameObject* ObjectInspector::findObjectAtPosition(Vector2 position)
    {
        // Buscar en la lista de GameObjects
        for (GameObject* obj : GameObject::gameObjects)
        {
            if (!obj) continue;
            
            // Calcular bounds del objeto (asumiendo que la textura está centrada)
            Rectangle objBounds = {
                obj->position.x - obj->texture.width / 2.0f,
                obj->position.y - obj->texture.height / 2.0f,
                static_cast<float>(obj->texture.width),
                static_cast<float>(obj->texture.height)
            };
            
            if (CheckCollisionPointRec(position, objBounds))
            {
                return obj;
            }
        }
        
        return nullptr;
    }
    
    std::string ObjectInspector::formatVector2(Vector2 vec)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << vec.x << "," << vec.y;
        return ss.str();
    }
    
    Vector2 ObjectInspector::parseVector2(const std::string& str)
    {
        Vector2 result = {0.0f, 0.0f};
        
        size_t commaPos = str.find(',');
        if (commaPos != std::string::npos)
        {
            try
            {
                result.x = std::stof(str.substr(0, commaPos));
                result.y = std::stof(str.substr(commaPos + 1));
            }
            catch (const std::exception&)
            {
                // Si hay error en el parsing, devolver posición original
                if (selectedObject)
                {
                    result = selectedObject->position;
                }
            }
        }
        
        return result;
    }
    
    void ObjectInspector::setColors(Color window, Color text, Color header)
    {
        windowColor = window;
        textColor = text;
        headerColor = header;
    }
}