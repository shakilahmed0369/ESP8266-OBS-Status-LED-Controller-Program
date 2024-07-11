# ESP8266 OBS Status LED Controller

This Arduino sketch enables an ESP8266 device to control an LED based on the streaming status from OBS (Open Broadcaster Software) via WebSocket communication.

## Requirements

- ESP8266 board (e.g., NodeMCU ESP-12E)
- Arduino IDE with necessary libraries:
  - ESP8266WiFi
  - WebSocketsClient
  - ArduinoJson

## Setup

1. Replace the placeholders in the code with your network credentials (`ssid` and `password`) and OBS WebSocket server details (`host`, `port`, `wsPath`).
2. Upload the sketch to your ESP8266 board using the Arduino IDE.
3. Ensure OBS WebSocket is enabled and configured with appropriate authentication if required.

## Functionality

- The ESP8266 connects to your WiFi network and establishes a WebSocket connection with OBS.
- It periodically checks the streaming status from OBS.
- Depending on the status (`RecordingStarted`, `RecordingStopped`, `RecordingPaused`, `RecordingResumed`), it controls an LED connected to pin D1:
  - LED blinks at a set interval when recording is active and not paused.
  - LED remains off when recording is inactive.
  - LED stays on when recording is paused.

## Notes

- Ensure your ESP8266 is compatible with the libraries used and has adequate power supply for stable operation.
- This sketch provides basic functionality and can be expanded for additional features or integrations as needed.

