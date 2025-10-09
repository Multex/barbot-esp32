#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <SD.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>

#include "config.h"
#include "state.h"
#include "web_page.h"
#include "motor_control.h"
#include "recipe_handler.h"

// Variables compartidas que controlan el estado del sistema
BarbotState currentState = IDLE;
String currentRecipe = "";
String statusMessage = "Listo para preparar bebidas";
int currentDispenser = 1;
long currentSteps = 0;
JsonDocument doc;

WebServer server(80);

String readFileToString(fs::FS &fs, const char *path) {
  File file = fs.open(path, FILE_READ);
  if (!file) {
    Serial.printf("Error abriendo %s\n", path);
    return "";
  }
  String s;
  while (file.available()) s += char(file.read());
  file.close();
  return s;
}

void handleRoot() {
  server.send_P(200, "text/html", PAGE_MAIN);
}

void handleStatus() {
  JsonDocument statusDoc;
  const char* stateNames[] = {"IDLE", "HOMING", "RECIPE_START", "RECIPE_MOVE", "RECIPE_SERVE", "RECIPE_WAIT_FINISH", "RECIPE_RETURN", "RECIPE_DONE"};
  statusDoc["state"] = stateNames[currentState];
  statusDoc["message"] = statusMessage;
  statusDoc["dispenser"] = currentDispenser;
  statusDoc["recipe"] = currentRecipe;
  String json;
  serializeJson(statusDoc, json);
  server.send(200, "application/json", json);
}

void handleRecipes() {
  String json = readFileToString(SD, "/recipes.json");
  server.send(200, "application/json", json);
}

void handlePrepare() {
  if (currentState != IDLE) {
    server.send(400, "text/plain", "Barbot ocupado");
    return;
  }
  String recipeName = server.arg("recipe");
  if (recipeName.isEmpty() || doc[recipeName].isNull()) {
    server.send(404, "text/plain", "Receta no encontrada");
    return;
  }
  server.send(200, "text/plain", "Iniciando " + recipeName);
  startRecipe(recipeName);
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Barbot Inicializando ===");
  
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
  
  WiFiManager wm;
  
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    Serial.println("Botón presionado. Reseteando configuración WiFi...");
    wm.resetSettings();
    Serial.println("Configuración reseteada. Reiniciando en 3 segundos...");
    delay(3000);
    ESP.restart();
  }

  setupMotors();
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD mount failed!");
    while (1);
  }
  
  String json = readFileToString(SD, "/recipes.json");
  if (deserializeJson(doc, json)) {
    Serial.println("JSON parse failed!");
    while (1);
  }
  
  homeBase();
  disableAllMotors();

  wm.setConnectTimeout(15);

  Serial.println("Intentando conectar al WiFi guardado...");
  if (!wm.autoConnect("BarBot_Config")) {
    Serial.println("Fallo al conectar. Reiniciando en 5 segundos...");
    delay(5000);
    ESP.restart();
  }
  
  Serial.println("\nWiFi conectado! IP: " + WiFi.localIP().toString());

  if (!MDNS.begin("barbot")) {
    Serial.println("Error iniciando mDNS!");
  } else {
    MDNS.addService("http", "tcp", 80);
    Serial.println("mDNS iniciado. Accede desde http://barbot.local");
  }

  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/recipes", handleRecipes);
  server.on("/prepare", HTTP_POST, handlePrepare);
  server.begin();
  
  Serial.println("=== BARBOT LISTO ===");
}

// Bucle principal: atiende peticiones HTTP y avanza la receta
void loop() {
  server.handleClient();
  updateRecipe();
  
  if (Serial.available() && currentState == IDLE) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (!line.isEmpty() && !doc[line].isNull()) {
      startRecipe(line);
    }
  }
}