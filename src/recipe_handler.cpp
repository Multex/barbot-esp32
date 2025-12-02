#include "recipe_handler.h"
#include "motor_control.h"
#include "state.h"

// Estado interno para llevar la receta en curso
static int currentIngredientIndex = 0;
static JsonArray ingredients;
static unsigned long waitStartTime = 0;
static unsigned long initialDelayTime = 0;

void startRecipe(String recipeName) {
  // Inicializa la receta y guarda sus ingredientes
  currentState = RECIPE_START;
  currentRecipe = recipeName;

  // Buscar la receta en el array "recipes"
  JsonArray recipes = doc["recipes"].as<JsonArray>();
  bool found = false;
  for (JsonObject r : recipes) {
    if (r["id"].as<String>() == recipeName) {
      ingredients = r["steps"].as<JsonArray>();
      found = true;
      break;
    }
  }

  if (!found) {
    Serial.printf("❌ Error: Receta '%s' no encontrada en el JSON. Recetas "
                  "disponibles:\n",
                  recipeName.c_str());
    for (JsonObject r : recipes) {
      Serial.printf(" - %s\n", r["id"].as<const char *>());
    }
    currentState = IDLE;
    statusMessage = "Error: Receta no encontrada";
    return;
  }

  currentIngredientIndex = 0;

  // Publica el mensaje inicial para el panel web
  statusMessage = "Preparando tu bebida...";
  Serial.println(">> Estado: RECIPE_START, Mensaje: Preparando...");
  initialDelayTime = millis(); // Marca un breve retardo antes de mover la base
}

// Se ejecuta desde loop() para avanzar la máquina de estados
void updateRecipe() {
  // Salir si no hay receta activa
  if (currentState < RECIPE_START) {
    return;
  }

  switch (currentState) {
  case RECIPE_START:
    // Retardo no bloqueante para dar tiempo a la UI
    if (millis() - initialDelayTime >= 250) {
      currentState = RECIPE_MOVE;
    }
    break;

  case RECIPE_MOVE: {
    if (currentIngredientIndex >= ingredients.size()) {
      currentState =
          RECIPE_WAIT_FINISH; // Sin ingredientes pendientes, pasamos al cierre
      break;
    }
    JsonObject ing = ingredients[currentIngredientIndex];
    Serial.printf("   Moviendo a dispensador %d\n", (int)ing["disp"]);
    gotoDispenser(ing["disp"]);
    currentState = RECIPE_SERVE;
    break;
  }

  case RECIPE_SERVE: {
    JsonObject ing = ingredients[currentIngredientIndex];
    Serial.printf("   Sirviendo %d oz\n", (int)ing["oz"]);
    serveOz(ing["oz"]);
    currentIngredientIndex++;
    currentState = RECIPE_MOVE;
    break;
  }

  case RECIPE_WAIT_FINISH:
    statusMessage = "✅ ¡Bebida lista! Puede retirar su vaso.";
    Serial.println(">> Estado: RECIPE_WAIT_FINISH, Mensaje: Bebida lista");
    waitStartTime = millis();
    currentState = RECIPE_RETURN;
    break;

  case RECIPE_RETURN:
    // Temporizador de 7 s antes de volver al origen
    if (millis() - waitStartTime >= 7000) {
      statusMessage = "Regresando a la posicion inicial...";
      Serial.println(">> Estado: RECIPE_RETURN, Mensaje: Regresando");
      gotoDispenser(1);
      disableAllMotors();
      currentState = RECIPE_DONE;
    }
    break;

  case RECIPE_DONE:
    statusMessage = "Listo para preparar bebidas";
    Serial.println(">> Estado: IDLE, Mensaje: Listo");
    currentState = IDLE;
    currentRecipe = "";
    break;
  }
}