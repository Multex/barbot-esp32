#pragma once

// Configuración de pines
#define SD_CS_PIN 5
#define ENDSTOP_PIN 13
#define RESET_BUTTON_PIN 0 // Pin IO0, usualmente el botón "BOOT"

// Driver de la base (L298N #1)
#define BASE_PIN_1 25
#define BASE_PIN_2 26
#define BASE_PIN_3 27
#define BASE_PIN_4 14
#define BASE_ENABLE_PIN 32

// Driver del dispensador (L298N #2)
#define SERVE_PIN_1 2
#define SERVE_PIN_2 4
#define SERVE_PIN_3 16
#define SERVE_PIN_4 17
#define SERVE_ENABLE_PIN 33

// Parámetros de los motores
#define STEPS_PER_REVOLUTION 200
#define TOTAL_VUELTAS 45
#define TOTAL_DISPENSADORES 5