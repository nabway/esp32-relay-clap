#include <WiFi.h>
#include <HTTPClient.h>
#include "config/secrets.hpp"
#include "config/pins.hpp"

// just test toggling every 10s for now to confirm both devices can talk to
// each other. After that I will include MQTT with telegram version (v0.3)
const unsigned long TOGGLE_INTERVAL_MS = 10000;
unsigned long lastToggle = 0;

void toggleRelay() {
  HTTPClient http;
  String url = "http://" + String(secrets::PLUG_IP) + "/switch/switch/toggle";
  http.begin(url);
  http.addHeader("Content-Length", "0");

  int httpCode = http.POST("");
  if (httpCode == 200) {
    log_i("Relay toggled OK");
  } else {
    log_e("Toggle FAILED, HTTP code: %d", httpCode);
  }

  http.end();
}

void toggleLed() {
  digitalWrite(pins::ONBOARD_LED, !digitalRead(pins::ONBOARD_LED));
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(secrets::WIFI_SSID_2, secrets::WIFI_PASSWORD_2);

  pinMode(pins::ONBOARD_LED, OUTPUT);
  digitalWrite(pins::ONBOARD_LED, HIGH);  // start OFF
  
  log_i("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }

  log_i("Connected, IP: %s", WiFi.localIP().toString().c_str());
}

void loop() {
  if (millis() - lastToggle >= TOGGLE_INTERVAL_MS) {
    toggleRelay();
    toggleLed();
    lastToggle = millis();
  }
}