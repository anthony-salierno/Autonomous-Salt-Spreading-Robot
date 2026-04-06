#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "wifi_utils.h"

bool wifiChipOn = false;


void enableWifi() {
    if (wifiChipOn) return; // Already on
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected, IP: ");
    Serial.println(WiFi.localIP());
    wifiChipOn = true;
}

void disableWifi() {
    if (!wifiChipOn) return; // Already off
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("WiFi disabled");
    wifiChipOn = false;
}
