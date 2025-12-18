/*
 * BasicUsage Example
 *
 * Demonstrates basic usage of the Updatable library.
 * Creates two LED blinkers that update at different rates.
 */

#include <Updatable.h>

class LEDBlinker : public Updatable {
private:
  int pin;
  long int interval;
  long int elapsed;
  bool state;

public:
  LEDBlinker(int ledPin, long int blinkInterval) {
    pin = ledPin;
    interval = blinkInterval;
    elapsed = 0;
    state = false;
    pinMode(pin, OUTPUT);
  }

  void update(long int deltaTime) override {
    elapsed += deltaTime;

    if (elapsed >= interval) {
      elapsed = 0;
      state = !state;
      digitalWrite(pin, state ? HIGH : LOW);

      if (DebugMode()) {
        Serial.print("LED on pin ");
        Serial.print(pin);
        Serial.print(" toggled to ");
        Serial.println(state ? "ON" : "OFF");
      }
    }
  }
};

// Create two LED blinkers with different intervals
LEDBlinker led1(LED_BUILTIN, 1000);  // Blink every 1 second
LEDBlinker led2(12, 500);            // Blink every 0.5 seconds

void setup() {
  Serial.begin(9600);

  // Optional: Enable debug mode to see update messages
  // Updatable::setDebugMode(true);

  Serial.println("Updatable Library - Basic Usage Example");
  Serial.println("Two LEDs blinking at different rates");
}

void loop() {
  static unsigned long lastUpdate = 0;
  unsigned long now = millis();
  long int deltaTime = now - lastUpdate;
  lastUpdate = now;

  // Update all registered Updatable instances
  Updatable::updateAllInstances(deltaTime);

  delay(10);  // Small delay to prevent excessive looping
}
