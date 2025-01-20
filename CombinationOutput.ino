#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"

// WiFi credentials
const char* ssid = "";
const char* password = "";

// Motor 1
const int motor1Pin1 = 26; 
const int motor1Pin2 = 27; 

// Motor 2
const int motor2Pin1 = 33;  
const int motor2Pin2 = 32;  

// Stepper motor 
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

  // Motor pins setup
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);

  // Initially disable all motors
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, HIGH);
    digitalWrite(ENABLE_PIN, HIGH); // Disable stepper

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

        // Determine action based on product_id
        switch (productId) {
        case 1:
            stepperDuration = 3; 
            startStepper();
            delay(2000);      
            moveMotor1Forward();
            break;
        case 2:
            stepperDuration = 4; 
            startStepper(); 
            delay(2000);      
            moveMotor2Forward(); 
            break;
        case 3:
            digitalWrite(33, HIGH);
            digitalWrite(32, LOW);
            digitalWrite(26, HIGH);
            digitalWrite(27, LOW);
            delay(2000);
        default:
            Serial.println("Invalid product_id received");
            data["error"] = "Invalid product_id";
            break;
        }

        data["message"] = "Motor action initiated";
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

void moveMotor1Forward() {
    Serial.println("Moving Motor 1 forward...");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    delay(3500); // Move forward for 4 seconds
    Serial.println("Stopping Motor 1...");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, HIGH);
}

void moveMotor2Forward() {
    Serial.println("Moving Motor 2 forward...");
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(3500); // Move forward for 4 seconds
    Serial.println("Stopping Motor 2...");
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, HIGH);
}

void startStepper() {
    digitalWrite(ENABLE_PIN, LOW);
    digitalWrite(DIR_PIN, HIGH);
    stepperStartTime = millis();
    stepperRunning = true;
    Serial.printf("Starting stepper motor for %d seconds...\n", stepperDuration);
}

void makeStep() {
    static unsigned long lastStepTime = 0;
    unsigned long currentStepTime = millis();
    if (currentStepTime - lastStepTime >= 1) { 
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
        Serial.println("Stepper motor stopped.");
    }
    // Handle other tasks
}
