# 🛡️ Sistema de Manejo de Excepciones - Ecos del Crepúsculo

## 📋 Resumen

Hemos implementado un **sistema robusto de manejo de excepciones** con try-catch-throw que captura y maneja errores en tiempo de ejecución de manera elegante, integrado completamente con nuestro sistema de debug existente.

## 🏗️ Arquitectura del Sistema

### 🎯 **Clases de Excepción Personalizadas**

#### 1. **GameException** (Clase Base)
```cpp
class GameException : public std::exception {
    // Información automática: mensaje, archivo, línea, función
    // Método: getDetailedMessage() para información completa
}
```

#### 2. **CriticalGameException**
```cpp
// Para errores críticos que requieren atención inmediata
THROW_CRITICAL_EXCEPTION("Error crítico del sistema");
```

#### 3. **ResourceException**
```cpp
// Para errores de carga/acceso de recursos
THROW_RESOURCE_EXCEPTION("Textura no encontrada", "path/to/texture.png");
```

#### 4. **MemoryException**
```cpp
// Para errores de asignación y gestión de memoria
THROW_MEMORY_EXCEPTION("Memoria insuficiente", 1024*1024, 512*1024);
```

#### 5. **PerformanceException**
```cpp
// Para errores de rendimiento crítico
THROW_PERFORMANCE_EXCEPTION("FPS crítico", 12.5f, 30.0f);
```

#### 6. **GameLogicException**
```cpp
// Para errores de lógica del juego y validación
THROW_LOGIC_EXCEPTION("Valor inválido", "1-100", "150");
```

## 🔧 Macros y Herramientas

### **Macros para Lanzar Excepciones**
```cpp
THROW_GAME_EXCEPTION(msg)           // Excepción general
THROW_CRITICAL_EXCEPTION(msg)       // Excepción crítica
THROW_RESOURCE_EXCEPTION(msg, path) // Excepción de recurso
THROW_MEMORY_EXCEPTION(msg, req, av) // Excepción de memoria
THROW_PERFORMANCE_EXCEPTION(msg, cur, min) // Excepción de rendimiento
THROW_LOGIC_EXCEPTION(msg, exp, act) // Excepción de lógica
```

### **Macros para Manejo de Excepciones**
```cpp
HANDLE_GAME_EXCEPTION(ex)    // Manejar excepción del juego
HANDLE_STD_EXCEPTION(ex)     // Manejar excepción estándar
HANDLE_UNKNOWN_EXCEPTION()   // Manejar excepción desconocida
```

### **Macro para Ejecución Segura**
```cpp
SAFE_EXECUTE({
    // Código que puede lanzar excepciones
    GameObject* obj = new GameObject();
    obj->initialize();
});
```

## 🎮 Sistema de Captura Integrado

### **Main Loop Protegido**
Todo el bucle principal del juego está envuelto en un sistema completo de try-catch:

```cpp
while (!WindowShouldClose()) {
    try {
        // Lógica del juego
        
    } catch (const CriticalGameException& ex) {
        HANDLE_GAME_EXCEPTION(ex);
    } catch (const ResourceException& ex) {
        HANDLE_GAME_EXCEPTION(ex);
    } catch (const MemoryException& ex) {
        HANDLE_GAME_EXCEPTION(ex);
    } catch (const PerformanceException& ex) {
        HANDLE_GAME_EXCEPTION(ex);
    } catch (const GameLogicException& ex) {
        HANDLE_GAME_EXCEPTION(ex);
    } catch (const std::bad_alloc& ex) {
        HANDLE_STD_EXCEPTION(ex);
    } catch (const std::runtime_error& ex) {
        HANDLE_STD_EXCEPTION(ex);
    } catch (const std::exception& ex) {
        HANDLE_STD_EXCEPTION(ex);
    } catch (...) {
        HANDLE_UNKNOWN_EXCEPTION();
    }
}
```

## 🎯 Simulaciones de Excepciones

### **Teclas de Prueba Disponibles**

| Tecla | Tipo de Excepción | Descripción |
|-------|-------------------|-------------|
| **X** | CriticalGameException | Error crítico del sistema |
| **Z** | ResourceException | Recurso no encontrado |
| **V** | MemoryException | Fallo de asignación de memoria |
| **B** | PerformanceException | Rendimiento crítico |
| **N** | GameLogicException | Error de lógica del juego |
| **J** | std::runtime_error | Excepción estándar de C++ |
| **K** | Excepción desconocida | Tipo de excepción no manejado |

### **Ejemplo de Uso en el Juego**
```cpp
// Creación segura de GameObject
SAFE_EXECUTE({
    Texture2D objTexture = LoadTexture("mono.png");
    if (objTexture.id == 0) {
        THROW_RESOURCE_EXCEPTION("No se pudo cargar la textura", "mono.png");
    }
    GameObject* obj = new GameObject({200, 200}, "myObj", objTexture);
    if (!obj) {
        THROW_MEMORY_EXCEPTION("No se pudo crear el GameObject", sizeof(GameObject), 0);
    }
    GameObject::gameObjects.push_back(obj);
});
```

## 🔄 Flujo de Manejo de Excepciones

### **1. Detección y Lanzamiento**
```
Condición de error → THROW_*_EXCEPTION → Excepción lanzada
```

### **2. Captura Automática**
```
try-catch en main loop → Excepción capturada → Tipo identificado
```

### **3. Procesamiento**
```
HANDLE_*_EXCEPTION → Información extraída → Ventana de error mostrada
```

### **4. Recuperación**
```
Usuario elige opción → Acción ejecutada → Juego continúa/reinicia/termina
```

## 💡 Información Automática Capturada

### **Para Cada Excepción:**
- **Mensaje de error** descriptivo
- **Archivo fuente** donde ocurrió
- **Número de línea** exacto
- **Función** donde se produjo
- **Tipo específico** de excepción
- **Información contextual** (paths, valores, etc.)

### **Ejemplo de Información Mostrada:**
```
[Error de Recurso] Recurso no encontrado o corrupto
Archivo: src/main.cpp:123
Función: loadGameAssets
Recurso: resources/missing_texture.png

Esto puede causar:
• Texturas faltantes en el juego
• Renderizado incorrecto
• Posibles crashes al acceder al recurso
```

## 🛠️ Integración con Sistema Existente

### **Compatibilidad Total**
- ✅ **Funciona junto** con el sistema CRITICAL_ERROR existente
- ✅ **Mismas ventanas** de diálogo profesionales
- ✅ **Mismas opciones** de recuperación (Continuar, Reiniciar, Salir)
- ✅ **Mismo logging** automático en consola

### **Ventajas Adicionales**
- 🎯 **Información más detallada** sobre el contexto del error
- 🔍 **Captura automática** de excepciones no previstas
- 🛡️ **Protección completa** del main loop
- 📊 **Clasificación automática** por tipo de error

## 🚀 Casos de Uso Reales

### **Durante Desarrollo**
```cpp
// Validación automática de recursos
if (!texture.id) {
    THROW_RESOURCE_EXCEPTION("Textura crítica no cargada", texturePath);
}

// Validación de memoria
if (!allocatedMemory) {
    THROW_MEMORY_EXCEPTION("Fallo crítico de memoria", requestedSize, availableSize);
}
```

### **En Producción**
```cpp
// Manejo elegante de errores inesperados
try {
    processGameLogic();
} catch (const GameException& ex) {
    // Error manejado elegantemente, juego continúa
    HANDLE_GAME_EXCEPTION(ex);
} catch (...) {
    // Cualquier error desconocido es capturado
    HANDLE_UNKNOWN_EXCEPTION();
}
```

### **Para Testing**
```cpp
// Simular condiciones de error específicas
if (IsKeyPressed(KEY_V)) {
    THROW_MEMORY_EXCEPTION("Simulación de memoria insuficiente", 1024*1024*100, 1024*1024*50);
}
```

## 📈 Beneficios del Sistema

### **🔒 Robustez**
- **Captura total** de excepciones en el main loop
- **Información detallada** para debugging efectivo
- **Recuperación elegante** sin crashes inesperados

### **🎮 Experiencia de Usuario**
- **Ventanas informativas** en lugar de crashes
- **Opciones de recuperación** claras
- **Continuidad del juego** cuando es posible

### **👨‍💻 Para Desarrolladores**
- **Debugging avanzado** con información contextual
- **Clasificación automática** de tipos de errores
- **Integración transparente** con código existente

### **🏭 Para Producción**
- **Estabilidad aumentada** mediante captura proactiva
- **Logging automático** para análisis post-mortem
- **Manejo profesional** de condiciones inesperadas

## 🎯 Próximas Mejoras Sugeridas

1. **Telemetría de Excepciones** - Envío automático de reportes de error
2. **Stack Trace Automático** - Captura de la pila de llamadas
3. **Excepciones de Red** - Para errores de conectividad
4. **Excepciones de Audio** - Para errores del sistema de sonido
5. **Dashboard de Errores** - Interfaz web para monitoreo

## ✅ Estado Actual

**COMPLETAMENTE IMPLEMENTADO Y FUNCIONAL** ✅

- ✅ 6 tipos de excepciones personalizadas
- ✅ 7 simulaciones de prueba (teclas X, Z, V, B, N, J, K)
- ✅ Main loop completamente protegido
- ✅ Integración total con sistema de ventanas de error
- ✅ Macros de conveniencia para uso fácil
- ✅ Documentación completa
- ✅ Compilación exitosa sin errores

¡El sistema está listo para uso en desarrollo y producción! 🚀