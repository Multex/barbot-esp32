# Barbot ESP32

Barbot es un dispensador de bebidas automatizado basado en ESP32. Mueve una base circular entre varios dispensadores, lee recetas desde una tarjeta SD y expone un panel web en español para elegir bebidas desde el navegador.

## Características

- Configuración WiFi con portal cautivo vía WiFiManager (botón de reset durante el arranque).
- Servidor web con UI en español y mDNS en `http://barbot.local`.
- Máquina de estados que gestiona homing, movimiento entre dispensadores y servido.
- Recetas en `recipes.json` (formato simple con `disp` y `oz`).
- Control de motores paso a paso (Arduino `Stepper`) para base y dispensador.
- Consola serie para ejecutar recetas manualmente cuando el sistema está en reposo.

## Demo rápida

1) Enciende la placa con SD insertada. 2) Conecta a WiFi guardada o portal `BarBot_Config`. 3) Abre `http://barbot.local` y elige una receta.

## Requisitos

- Placa `esp32doit-devkit-v1`
- PlatformIO (VSCode o CLI)
- Tarjeta SD con `recipes.json`

## Comandos

- Compilar: `platformio run`
- Subir firmware: `platformio run --target upload`
- Monitor serie: `platformio device monitor --baud 115200`
- Pruebas: `platformio test -e esp32doit-devkit-v1`
- Limpiar: `platformio run --target clean`

## Estructura

- `src/main.cpp`: servidor HTTP y máquina de estados principal.
- `src/motor_control.cpp`: homing, movimientos y servido.
- `src/recipe_handler.cpp`: lógica de ejecución de recetas.
- `include/*.h`: pines/config, estados y recursos web.
- `recipes.json`: catálogo de recetas que muestra la UI.
- `test/`: suites de pruebas (Unity/PlatformIO).

## Mapeo de pines (config.h)

- SD: `SD_CS_PIN 5`
- Endstop: `ENDSTOP_PIN 13`
- Botón reset WiFi: `RESET_BUTTON_PIN 0`
- Base (L298N #1): `25, 26, 27, 14` y `BASE_ENABLE_PIN 32`
- Dispensador (L298N #2): `2, 4, 16, 17` y `SERVE_ENABLE_PIN 33`

Parámetros: `STEPS_PER_REVOLUTION 200`, `TOTAL_VUELTAS 45`, `TOTAL_DISPENSADORES 5`.

## Recetas (recipes.json)

Ejemplo:

```json
{
  "VodkaCola": [
    { "disp": 2, "oz": 1 },
    { "disp": 1, "oz": 2 }
  ]
}
```

- `disp`: índice de dispensador (1..TOTAL_DISPENSADORES)
- `oz`: onzas a servir por paso.

Consulta `docs/RECIPES.md` para detalles y recomendaciones.

## Flujo de operación

1. Homing de la base al encender (endstop).
2. Carga de recetas desde SD (`/recipes.json`).
3. Conexión WiFi (o portal cautivo si se mantiene presionado el botón BOOT al inicio).
4. UI web en `http://barbot.local` para iniciar recetas.
5. Estado final: vuelve al dispensador 1 y deshabilita motores.

## Desarrollo

- Estilo: indentación de 2 espacios, llaves en misma línea, cadenas de UI en español.
- Evitar asignación dinámica en rutas críticas; reutiliza buffers si es posible.
- Ejecuta `platformio test` antes de subir cambios importantes.
- Ver `CONTRIBUTING.md` para commits y PRs.

## Seguridad y notas

- No hardcodees credenciales: el portal de WiFiManager gestiona la configuración.
- Valida ingredientes y volúmenes en `recipes.json` antes de uso.
- Ten cuidado con líquidos y electricidad: protege controladores y asegúrate de ventilación.

## Imágenes

Coloca tus fotos y diagramas en `images/` y enlázalos aquí.

Ejemplos de nombres sugeridos:

- `images/final-assembled.jpg` — foto del proyecto terminado
- `images/schematic.png` — esquemático electrónico
- `images/wiring.jpg` — cableado real
- `images/web-ui.png` — captura de la interfaz web

Ejemplo de uso en Markdown:

```md
![Proyecto final](images/final-assembled.jpg)
![Esquemático](images/schematic.png)
```

Sugerencias: exporta a 1600–2400 px de ancho y comprime (JPG/PNG) para mantener el repo liviano.

## Licencia

MIT — ver `LICENSE` para los términos completos.

## Créditos

- Basado en ESP32 + Arduino, con `ArduinoJson`, `Stepper` y `WiFiManager`.
