#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

// Estados utilizados por la máquina principal
enum BarbotState {
  IDLE,
  HOMING,
  
  RECIPE_START,
  RECIPE_MOVE,
  RECIPE_SERVE,
  RECIPE_WAIT_FINISH,
  RECIPE_RETURN,
  RECIPE_DONE
};

// Variables globales compartidas con el resto de módulos
extern BarbotState currentState;
extern String currentRecipe;
extern String statusMessage;
extern int currentDispenser;
extern long currentSteps;
extern JsonDocument doc;