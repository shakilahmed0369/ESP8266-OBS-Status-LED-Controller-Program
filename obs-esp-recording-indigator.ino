#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = ""; // your wifi name
const char* password = ""; // your wifi password

// Replace with OBS WebSocket server details
const char* host = ""; // e.g., "192.168.1.100"
const uint16_t port = 4444;
const char* wsPath = "/";

unsigned long previousMillis = 0;
unsigned long interval = 1000;
bool recording = false;
bool pause = false;

WebSocketsClient webSocket;

const int ledPin = D1;

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to OBS WebSocket
  webSocket.begin(host, port, wsPath);
  webSocket.onEvent(webSocketEvent);
  webSocket.setAuthorization("", "password"); // Replace with OBS WebSocket password if set

  // Send initial request to get recording status
  sendWebSocketMessage("{\"request-type\": \"GetStreamingStatus\", \"message-id\": \"1\"}");
}

void loop() {
  // Handle interval blinking or other tasks here
  unsigned long currentMillis = millis();
  if (recording && !pause) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      digitalWrite(ledPin, !digitalRead(ledPin));
    }
  }else if (pause){
    digitalWrite(ledPin, HIGH);
  }else {
    digitalWrite(ledPin, LOW);
  }

  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("Disconnected from WebSocket");
      break;
    case WStype_CONNECTED:
      Serial.println("Connected to WebSocket");
      break;
    case WStype_TEXT:
      Serial.printf("Received text: %s\n", payload);
      handleWebSocketMessage(payload);
      break;
  }
}

void sendWebSocketMessage(String message) {
  webSocket.sendTXT(message);
}

void handleWebSocketMessage(uint8_t * payload) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.println("Failed to parse JSON");
    return;
  }

  if (doc.containsKey("update-type")) {
    // Status update
    String updateType = doc["update-type"].as<String>();
    if (updateType == "RecordingStarted") {
      recording = true;
      pause = false;
    } else if (updateType == "RecordingStopped") {
      recording = false;
      pause = false;
    } else if (updateType == "RecordingPaused") {
      pause = true;
    } else if (updateType == "RecordingResumed") {
      pause = false;
    }
  }
}
