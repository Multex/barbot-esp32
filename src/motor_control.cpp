#include "motor_control.h"
#include "config.h"
#include "state.h"
#include <Arduino.h>
#include <Stepper.h>

const int basePins[4] = {BASE_PIN_1, BASE_PIN_2, BASE_PIN_3, BASE_PIN_4};
const int servePins[4] = {SERVE_PIN_1, SERVE_PIN_2, SERVE_PIN_3, SERVE_PIN_4};
const long pasosPorDisp =
    (long)(STEPS_PER_REVOLUTION * TOTAL_VUELTAS) / (TOTAL_DISPENSADORES - 1);

Stepper baseStepper(STEPS_PER_REVOLUTION, basePins[0], basePins[1], basePins[2],
                    basePins[3]);
Stepper serveStepper(STEPS_PER_REVOLUTION, servePins[0], servePins[1],
                     servePins[2], servePins[3]);

void setupMotors() {
  pinMode(BASE_ENABLE_PIN, OUTPUT);
  pinMode(SERVE_ENABLE_PIN, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(basePins[i], OUTPUT);
    pinMode(servePins[i], OUTPUT);
  }
  pinMode(ENDSTOP_PIN, INPUT_PULLUP);

  baseStepper.setSpeed(190);
  serveStepper.setSpeed(35);
}

void disableAllMotors() {
  digitalWrite(BASE_ENABLE_PIN, LOW);
  digitalWrite(SERVE_ENABLE_PIN, LOW);
  digitalWrite(basePins[0], LOW);
  digitalWrite(basePins[1], LOW);
  digitalWrite(basePins[2], LOW);
  digitalWrite(basePins[3], LOW);
  digitalWrite(servePins[0], LOW);
  digitalWrite(servePins[1], LOW);
  digitalWrite(servePins[2], LOW);
  digitalWrite(servePins[3], LOW);
}

void homeBase() {
  currentState = HOMING;
  statusMessage = "Calibrando posicion...";
  Serial.println("🏃‍♂️ Iniciando secuencia de Homing para la base...");

  if (digitalRead(ENDSTOP_PIN) == LOW) {
    Serial.println("📍 Endstop ya presionado. Barbot en posicion inicial.");
    currentSteps = 0;
    currentDispenser = 1;
    currentState = IDLE;
    statusMessage = "Listo para preparar bebidas";
    return;
  }

  digitalWrite(BASE_ENABLE_PIN, HIGH);
  delay(10);

  while (digitalRead(ENDSTOP_PIN) == HIGH) {
    baseStepper.step(-1);
  }

  digitalWrite(BASE_ENABLE_PIN, LOW);
  digitalWrite(basePins[0], LOW);
  digitalWrite(basePins[1], LOW);
  digitalWrite(basePins[2], LOW);
  digitalWrite(basePins[3], LOW);

  Serial.println("✅ ¡Posicion inicial encontrada!");
  currentSteps = 0;
  currentDispenser = 1;
  currentState = IDLE;
  statusMessage = "Listo para preparar bebidas";
}

void gotoDispenser(int d) {
  if (d < 1 || d > TOTAL_DISPENSADORES) {
    Serial.printf("❌ Dispensador %d fuera de rango\n", d);
    return;
  }

  long target = (long)(d - 1) * pasosPorDisp;
  long delta = target - currentSteps;

  if (delta == 0) {
    currentDispenser = d;
    return;
  }

  digitalWrite(BASE_ENABLE_PIN, HIGH);
  delay(10);

  baseStepper.step(delta);

  digitalWrite(BASE_ENABLE_PIN, LOW);
  digitalWrite(basePins[0], LOW);
  digitalWrite(basePins[1], LOW);
  digitalWrite(basePins[2], LOW);
  digitalWrite(basePins[3], LOW);

  currentSteps = target;
  currentDispenser = d;
}

void serveOz(int oz) {
  if (oz <= 0)
    return;

  digitalWrite(SERVE_ENABLE_PIN, HIGH);
  delay(10);

  for (int i = 0; i < oz; i++) {
    serveStepper.step(-STEPS_PER_REVOLUTION * 3);

    // Non-blocking delay (3000ms)
    unsigned long start = millis();
    while (millis() - start < 3000) {
      yieldAndHandle();
    }

    serveStepper.step(STEPS_PER_REVOLUTION * 3);

    // Non-blocking delay (2000ms)
    start = millis();
    while (millis() - start < 2000) {
      yieldAndHandle();
    }
  }

  digitalWrite(SERVE_ENABLE_PIN, LOW);
  digitalWrite(servePins[0], LOW);
  digitalWrite(servePins[1], LOW);
  digitalWrite(servePins[2], LOW);
  digitalWrite(servePins[3], LOW);
}