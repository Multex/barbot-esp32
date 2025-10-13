# Desarrollo

## Entorno

- PlatformIO con `esp32doit-devkit-v1` (ver `platformio.ini`).
- Dependencias bloqueadas: `ArduinoJson`, `Stepper`, `WiFiManager`.

## Comandos útiles

- `platformio run` — compila.
- `platformio run --target upload` — sube firmware.
- `platformio device monitor --baud 115200` — logs por serie.
- `platformio test -e esp32doit-devkit-v1` — ejecuta pruebas.

## Rutas de código

- `src/main.cpp` — HTTP + estados.
- `src/motor_control.cpp` — motores base/dispensador.
- `src/recipe_handler.cpp` — ejecución de recetas.
- `include/*.h` — pines, estados, recursos web.

## Lineamientos

- Cadenas de UI en español.
- Evita new/delete en bucles de tiempo crítico.
- Coloca pines en `include/config.h` y documenta cambios.

