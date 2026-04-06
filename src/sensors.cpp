#include <Arduino.h>
#include "config.h"
#include "sensors.h"

double leftUltraSonicDistance  = 0.0;
double rightUltraSonicDistance = 0.0;
bool leftReflector  = false;
bool rightReflector = false;


void updateSensors() {
    leftReflector  = digitalRead(LEFT_REFLECTOR_PIN);
    rightReflector = digitalRead(RIGHT_REFLECTOR_PIN);

    leftUltraSonicDistance  = getDistanceToObstacle(LEFT_ULTRASONIC_TRIG_PIN, LEFT_ULTRASONIC_ECHO_PIN);
    rightUltraSonicDistance = getDistanceToObstacle(RIGHT_ULTRASONIC_TRIG_PIN, RIGHT_ULTRASONIC_ECHO_PIN);
}

double getDistanceToObstacle(int trigPin, int echoPin) {
    // Send a 10 microsecond pulse to trigger the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the duration of the echo pulse
    long duration = pulseIn(echoPin, HIGH, ULTRASONIC_TIMEOUT_US);

    if (duration == 0) {
        return ULTRASONIC_MAX_DIST_FT; // No echo received (out of range)
    }

    // Calculate distance in feet (speed of sound is approximately 1130 feet per second)
    return (duration / MICROSECONDS_PER_FOOT) / 12;
}

bool checkForObstacles() {
    return (leftUltraSonicDistance < OBSTACLE_DISTANCE_FT ||
            rightUltraSonicDistance < OBSTACLE_DISTANCE_FT ||
            leftReflector == HIGH ||
            rightReflector == HIGH);
}
