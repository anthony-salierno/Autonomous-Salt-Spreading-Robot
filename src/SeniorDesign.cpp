#include <Preferences.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "config.h"
#include "sensors.h"
#include "wifi_utils.h"


Preferences preferences;

int state        = 0;
int count        = 0;
bool returnMode  = false;
double positionX = 0.0;
double positionY = 0.0;
double drivewayLengthY = 0.0;
bool snowForecasted    = false;


// Forward declarations
void handleStateDocked();
void handleStateDriving();
void handleStateTurnLeft();
void handleStateTurnRight();
void handleStateReturnLeft();
void handleStateReturnRight();
bool forecastSnow();


void setup() {
    Serial.begin(115200);
    preferences.begin("startupData", false);
    drivewayLengthY = preferences.getDouble("driveway_length", 0.0);

    // Reflectors
    pinMode(LEFT_REFLECTOR_PIN, INPUT);
    pinMode(RIGHT_REFLECTOR_PIN, INPUT);

    // Ultrasonic sensors
    pinMode(LEFT_ULTRASONIC_ECHO_PIN, INPUT);
    pinMode(RIGHT_ULTRASONIC_ECHO_PIN, INPUT);
    pinMode(LEFT_ULTRASONIC_TRIG_PIN, OUTPUT);
    pinMode(RIGHT_ULTRASONIC_TRIG_PIN, OUTPUT);
}

void loop() {
    updateSensors();

    switch (state) {
        case 0: handleStateDocked();            break;
        case 1: handleStateDrivingStraight();   break;
        case 2: handleStateTurnLeft();          break;
        case 3: handleStateTurnRight();         break;
        case 4: handleStateReturnLeft();        break;
        case 5: handleStateReturnRight();       break;
    }
}


void handleStateDocked() {
    enableWifi();
    returnMode     = false;
    snowForecasted = false;
    // Battery Charging
    // Waiting to pull snow forecast data from the internet
    if (snowForecasted) {
        state = 1;
    }
}

void handleStateDrivingStraight() {
    disableWifi();
    if (returnMode) {
        // move forward, update position x (rows)
        // spread salt
        if (positionY == 0.0 && positionX == 0.0) {
            state = 0; // Go back to state 0 OR if the dock is detected by pogo pins also go back to state 0
        }
        if (checkForObstacles() && (count % 2 == 0)) {
            state = 2; // Go to state 2
        } else if (checkForObstacles() && !(count % 2 == 0)) {
            state = 3; // Go to state 3
        }
    } else {
        // move forward, update position x (rows)
        // spread salt
        if (checkForObstacles() && (count % 2 == 0) && (positionY + ROW_WIDTH_FT >= drivewayLengthY)) {
            state = 2; // Go to state 2
        } else if (checkForObstacles() && !(count % 2 == 0) && (positionY + ROW_WIDTH_FT >= drivewayLengthY)) {
            state = 3; // Go to state 3
        }
    }
}

void handleStateTurnLeft() {
    if (positionY + ROW_WIDTH_FT >= drivewayLengthY) {
        returnMode = true;
        // turn around
        state = 4; // Go to state 4
    } else {
        // turn left, move forward, update position y (columns), turn left
        // Spread salt
        count++;
        state = 1; // Go back to state 1
    }
}

void handleStateTurnRight() {
    if (positionY + ROW_WIDTH_FT >= drivewayLengthY) {
        returnMode = true;
        // turn around
        state = 5; // Go to state 5
    }
    // turn right, move forward, update position y (columns), turn right
    // Spread salt
    count++;
    state = 1; // Go back to state 1
}

void handleStateReturnLeft() {
    // turn right, move forward, turn right
    // Spread salt
    count--;
    state = 1; // Go back to state 1
}

void handleStateReturnRight() {
    // turn left, move forward, turn left
    // Spread salt
    count--;
    state = 1; // Go back to state 1
}

bool forecastSnow() {
    // Implement function to pull snow forecast data from the internet
    // Return true if snow is forecasted, false otherwise
    return false; // Placeholder return value
}
