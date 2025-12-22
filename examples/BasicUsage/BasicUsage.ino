/*
 * BasicUsage Example - Non-blocking LED Blinkers
 *
 * Demonstrates non-blocking programming with the Updatable library.
 * Two LEDs blink at different rates simultaneously without using delay().
 *
 * This example shows how to eliminate blocking delays and create
 * multiple concurrent tasks that run independently.
 */

#include <Updatable.h>

// Define LED_BUILTIN for platforms that don't have it
#ifndef LED_BUILTIN
  #if defined(ESP32)
    #define LED_BUILTIN 2  // ESP32 typically uses GPIO2
  #elif defined(ESP8266)
    #define LED_BUILTIN 2  // ESP8266 typically uses GPIO2
  #else
    #define LED_BUILTIN 13 // Default for most Arduino boards
  #endif
#endif

class LEDBlinker : public Updatable {
private:
  int pin;
  unsigned long interval;
  unsigned long elapsed;
  bool state;

public:
  LEDBlinker(int ledPin, unsigned long blinkInterval) {
    pin = ledPin;
    interval = blinkInterval;
    elapsed = 0;
    state = false;
    pinMode(pin, OUTPUT);
  }

  void update(unsigned long deltaTime) override {
    elapsed += deltaTime;

    if (elapsed >= interval) {
      elapsed -= interval;  // Preserve timing accuracy
      state = !state;
      digitalWrite(pin, state ? HIGH : LOW);

      if (debugMode()) {
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

  Serial.println("Updatable Library - Non-blocking Example");
  Serial.println("Two LEDs blinking independently without delay()");
  Serial.println("Notice: The loop stays responsive!");
}

void loop() {
  // Update all registered Updatable instances (non-blocking!)
  // Delta time is calculated automatically with overflow protection
  Updatable::updateAllInstances();

  // The loop stays responsive - you can add more non-blocking code here
  // No blocking delay() calls needed!

  // Note: You can also manually provide delta time if needed:
  // Updatable::updateAllInstances(deltaTime);
}
