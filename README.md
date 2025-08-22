# Ecos del Crepúsculo
Juego RPG desarrollado con Raylib y C++

## Compilación y Ejecución

### Requisitos Previos
- Windows 10/11
- Visual Studio 2022 (recomendado) o MinGW-w64
- Git (para clonar el repositorio)

### Compilación Rápida

#### Método Recomendado: Script Maestro
```bash
# Ejecutar el script maestro que te permite elegir el método
build-master.bat
```

#### Opción 1: CMake (Recomendado para problemas de ruta)
```bash
# Compilar con CMake
build-cmake.bat
```

#### Opción 2: Compilación Directa
```bash
# Compilación directa sin sistemas de build
build-direct.bat
```

#### Opción 3: Premake5 (Original)
```bash
# Compilar con Premake5 (puede fallar con caracteres especiales en ruta)
build-safe.bat

# O usar los scripts originales
build-VisualStudio2022.bat
build-MinGW-W64.bat
```

### Scripts Disponibles

#### Scripts de Compilación
- `build-master.bat` - Script maestro con múltiples opciones de build
- `build-cmake.bat` - Compila usando CMake (recomendado)
- `build-direct.bat` - Compilación directa sin sistemas de build
- `build-safe.bat` - Compila con Premake5 (versión mejorada)
- `build.bat` - Script de build original
- `build-mingw.bat` - Compila usando MinGW-w64

#### Scripts de Utilidad
- `clean.bat` - Limpia todos los archivos de compilación
- `dev.bat` - Modo desarrollo (compila y ejecuta automáticamente)
- `test.bat` - Ejecuta las pruebas del sistema

#### Scripts Originales
- `build-VisualStudio2022.bat` - Script original de Premake5 para VS2022
- `build-MinGW-W64.bat` - Script original de Premake5 para MinGW

### Estructura del Proyecto

```
Ecos-del-Crepusculo/
├── src/           # Código fuente principal
├── include/       # Archivos de cabecera
├── tests/         # Pruebas unitarias
├── resources/     # Recursos del juego (texturas, sonidos, etc.)
├── bin/           # Ejecutables compilados
├── build/         # Sistema de compilación (Premake5)
└── docs/          # Documentación
```

### Sistema de Debug

El juego incluye un sistema completo de debug con:

- **Logger**: Sistema de logging con múltiples niveles
- **Consola de Debug**: Consola in-game (F1 para abrir/cerrar)
- **Monitor de Rendimiento**: FPS, memoria, tiempo de frame
- **Inspector de Objetos**: Inspección en tiempo real de GameObjects
- **Procesador de Comandos**: Comandos de debug y desarrollo
- **Sistema de Configuración**: Configuración persistente

Ver `docs/DebugSystem_README.md` para más detalles.

### Controles de Debug

- `F1` - Abrir/cerrar consola de debug
- `F2` - Alternar monitor de rendimiento
- `F3` - Alternar inspector de objetos
- `~` - Enfocar línea de comandos de la consola

### Comandos de Consola Disponibles

- `help` - Mostrar todos los comandos disponibles
- `fps [target]` - Mostrar o establecer FPS objetivo
- `teleport <x> <y>` - Teletransportar jugador
- `spawn <type> <x> <y>` - Crear objeto en posición
- `inspect <id>` - Inspeccionar objeto por ID
- `config <key> [value]` - Ver o cambiar configuración
- `clear` - Limpiar consola
- `quit` - Salir del juego

### Configuración

El sistema guarda automáticamente la configuración en `debug_config.ini`. Puedes modificar:

- Niveles de logging
- Configuración de rendimiento
- Opciones de debug
- Configuraciones de optimización

### Desarrollo

Para desarrollo activo, usa:
```bash
dev.bat
```

Esto compilará automáticamente y ejecutará el juego cuando detecte cambios.

### Solución de Problemas

#### Problemas Comunes
1. **Caracteres especiales en ruta**: Si tienes problemas con acentos en la ruta, usa `build-cmake.bat` o `build-direct.bat`
2. **Error de premake5.exe**: Asegúrate de que `build/premake5.exe` existe
3. **Error de compilador**: Verifica que Visual Studio 2022, MinGW o CMake estén instalados
4. **Error de raylib**: Los scripts descargan automáticamente raylib, verifica tu conexión a internet
5. **Problemas de PATH**: Asegúrate de que el compilador esté en tu PATH del sistema

#### Métodos de Build Alternativos
- **Si Premake5 falla**: Usa CMake (`build-cmake.bat`)
- **Si CMake no está disponible**: Usa compilación directa (`build-direct.bat`)
- **Si tienes problemas de dependencias**: El script directo descarga y compila raylib automáticamente

#### Requisitos por Método
- **CMake**: Requiere CMake 3.16+ y Visual Studio o MinGW
- **Premake5**: Requiere Visual Studio 2022 o MinGW-w64
- **Directo**: Solo requiere un compilador C++ (cl.exe o g++.exe)
