#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"

// WiFi credentials
const char* ssid = "nashef";
const char* password = "fg62QMG4228FBASs";

// Motor control pins for Motor 1
const int motor1Pin1 = 26; // Extend pin
const int motor1Pin2 = 27; // Retract pin

// Motor control pins for Motor 2
const int motor2Pin1 = 2;  // Forward pin
const int motor2Pin2 = 4;  // Backward pin

// HTTP Server
AsyncWebServer server(80);

volatile int counter = 0; // Counter to track HTTP POST requests
int lastCounterValue = 0; // Tracks the last processed counter value

void moveMotor1Forward() {
  // Extend Motor 1
  Serial.println("Moving Motor 1 forward...");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  delay(4000); // Move forward for 4 seconds

  // Stop Motor 1
  Serial.println("Stopping Motor 1...");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, HIGH);
}

void moveMotor2Forward() {
  // Move Motor 2 forward
  Serial.println("Moving Motor 2 forward...");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(4000); // Move forward for 4 seconds

  // Stop Motor 2
  Serial.println("Stopping Motor 2...");
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, HIGH);
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "application/json", "{\"message\":\"Not found\"}");
}

void setup() {
  // Serial for debugging
  Serial.begin(115200);

  // Motor 1 pin setup
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, HIGH);

  // Motor 2 pin setup
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, HIGH);

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }

  Serial.print("Connected to WiFi. IP Address: ");
  Serial.println(WiFi.localIP());

  // HTTP POST handler
  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/post-message", [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<200> data;

    if (json.is<JsonObject>()) {
      JsonObject obj = json.as<JsonObject>();
      if (obj.containsKey("product_id")) {
        int productId = obj["product_id"];
        Serial.printf("Received product_id: %d\n", productId);

        // Move the corresponding motor based on product_id
        if (productId == 1) {
          moveMotor1Forward();
        } else if (productId == 2) {
          moveMotor2Forward();
        } else {
          Serial.println("Invalid product_id received");
          data["error"] = "Invalid product_id";
        }

        counter++; // Increment the counter
        data["message"] = "Motor action executed";
        data["counter"] = counter;
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
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // Main loop does nothing for now; actions are triggered by HTTP requests
}
