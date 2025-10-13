# Cableado

La asignación de pines está centralizada en `include/config.h`.

## SD (SPI)

- `SD_CS_PIN 5` (CS)
- MOSI/MISO/SCK según tu placa ESP32

## Endstop

- `ENDSTOP_PIN 13` (entrada con `INPUT_PULLUP`)

## Botón de reset WiFi

- `RESET_BUTTON_PIN 0` (IO0/BOOT)

## L298N Base (Driver #1)

- IN1 `BASE_PIN_1 25`
- IN2 `BASE_PIN_2 26`
- IN3 `BASE_PIN_3 27`
- IN4 `BASE_PIN_4 14`
- ENABLE `BASE_ENABLE_PIN 32`

## L298N Dispensador (Driver #2)

- IN1 `SERVE_PIN_1 2`
- IN2 `SERVE_PIN_2 4`
- IN3 `SERVE_PIN_3 16`
- IN4 `SERVE_PIN_4 17`
- ENABLE `SERVE_ENABLE_PIN 33`

Verifica el orden de fases del motor con el fabricante para evitar pasos erráticos. Si gira en sentido inverso, invierte el signo de los pasos o permuta fases.

