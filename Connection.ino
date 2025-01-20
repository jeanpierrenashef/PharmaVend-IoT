#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"

AsyncWebServer server(80);

const char* ssid = ""; 
const char* password = "";

int ledPin = -1; 
void handleLED(int productId) {

  ledPin = 2;
  if (productId == 1) {
    pinMode(ledPin, OUTPUT);
    for (int i = 0; i < 10; i++) { 
      digitalWrite(ledPin, HIGH);
      delay(50);
      digitalWrite(ledPin, LOW);
      delay(50);
    }
  } else if (productId == 2) {
      pinMode(ledPin, OUTPUT);
      for (int i = 0; i < 5; i++) { 
        digitalWrite(ledPin, HIGH);
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);
      }
  } else {
    Serial.println("Unknown product_id");
  }
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "application/json", "{\"message\":\"Not found\"}");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/post-message", [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<200> data;
    if (json.is<JsonObject>()) {
      JsonObject obj = json.as<JsonObject>();
      if (obj.containsKey("product_id")) {
        int productId = obj["product_id"];
        Serial.printf("Received product_id: %d\n", productId);
        handleLED(productId);

        data["message"] = "LED action executed";
        data["product_id"] = productId;
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
  // Main loop can be used for other tasks
}