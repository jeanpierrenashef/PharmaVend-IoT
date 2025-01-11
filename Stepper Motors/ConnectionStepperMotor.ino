#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"

// WiFi credentials
const char* ssid = "nashef";
const char* password = "fg62QMG4228FBASs";

// Stepper motor control pins
const int DIR_PIN = 15;
const int STEP_PIN = 2;
const int ENABLE_PIN = 13; // Enable pin for stepper motor

// HTTP Server
AsyncWebServer server(80);

bool stepperRunning = false;
unsigned long stepperStartTime;
int stepperDuration;

void setup() {
  Serial.begin(115200);
  
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, HIGH); // Initially disable the stepper

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  Serial.print("Connected to WiFi. IP Address: ");
  Serial.println(WiFi.localIP());

  AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/control-motor", [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<200> data;

    if (json.is<JsonObject>()) {
      JsonObject obj = json.as<JsonObject>();
      if (obj.containsKey("product_id")) {
        int productId = obj["product_id"];
        stepperDuration = (productId == 1) ? 3 : (productId == 2) ? 4 : 0;

        if (stepperDuration > 0) {
          startStepper();
          data["message"] = "Motor action initiated";
        } else {
          data["error"] = "Invalid product_id";
        }
      } else {
        data["error"] = "Missing product_id";
      }
    } else {
      data["error"] = "Invalid JSON";
    }

    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
    Serial.println(response);
  });

  server.addHandler(handler);
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "application/json", "{\"message\":\"Not found\"}");
  });
  server.begin();
}

void startStepper() {
    digitalWrite(ENABLE_PIN, LOW); // Enable the stepper
    digitalWrite(DIR_PIN, HIGH); // Set direction to clockwise
    stepperStartTime = millis();
    stepperRunning = true;
    Serial.printf("Starting motor for %d seconds...\n", stepperDuration);
}

void makeStep() {
  static unsigned long lastStepTime = 0;
  unsigned long currentStepTime = millis();
  if (currentStepTime - lastStepTime >= 1) { // Adjust the stepping interval as needed
    lastStepTime = currentStepTime;
    digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
  }
}

void loop() {
    if (stepperRunning && (millis() - stepperStartTime < stepperDuration * 1000)) {
        makeStep();
    } else if (stepperRunning) {
        digitalWrite(ENABLE_PIN, HIGH);
        stepperRunning = false;
        Serial.println("Motor stopped.");
    }
    // Perform other tasks
}
