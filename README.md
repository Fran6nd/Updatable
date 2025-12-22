# Updatable

A lightweight framework for **non-blocking Arduino programming**. Write clean, modular code without `delay()` calls.

## Description

The Updatable library enables true non-blocking programming in Arduino projects. Instead of scattering timing logic throughout your code with `millis()` checks and `delay()` calls, simply inherit from `Updatable` and implement your update logic. Objects automatically register themselves and receive periodic updates with delta time, allowing multiple components to run concurrently without blocking.

## Features

- **Non-blocking by design** - No more `delay()` blocking your program
- **Automatic registration** - Objects register themselves on construction
- **Delta time updates** - Each component receives time elapsed since last update
- **Batch processing** - Update all components with a single call
- **Zero configuration** - Just inherit and override `update()`
- **Debug mode support** - Enable debugging for all instances at once
- **Minimal overhead** - Lightweight and efficient

## Lifecycle Management

**Automatic registration and deregistration** - Objects register themselves on construction and safely deregister on destruction.

**What this means:**
- ✅ Perfect for persistent components created at startup that live throughout program execution
- ✅ Ideal for global objects, static instances, or objects with program lifetime
- ✅ Supports dynamically created/destroyed objects (e.g., with `new`/`delete`)
- ⚠️ **CRITICAL WARNING**: Never delete an `Updatable` object from within its own `update()` method or during `updateAllInstances()` execution, as this will modify the registry while it's being iterated, causing undefined behavior

**Best Practice:** For dynamic objects, manage their lifecycle outside of the update cycle (e.g., in your main loop after `updateAllInstances()` completes).

## Dependencies

This library now includes an **embedded Vector implementation** specifically designed for resource-constrained embedded devices. No external dependencies are required.

### About the Embedded Vector.h

The library includes a lightweight Vector implementation (based on [tomstewart89/Vector](https://github.com/tomstewart89/Vector/blob/master/Vector.h)) that replaces the need for external dependencies. This embedded version:

**Features:**
- ✅ Dynamic array with automatic resizing
- ✅ STL-like interface (PushBack, Size, Erase, etc.)
- ✅ Optimized for embedded systems
- ✅ No external dependencies

**Limitations:**
- ⚠️ Uses dynamic memory allocation (`new`/`delete`)
- ⚠️ No bounds checking on direct access (returns `OB` member on out-of-bounds)
- ⚠️ Reallocation may fragment memory on constrained devices
- ⚠️ Not thread-safe

**Dynamic Allocation Considerations for Embedded Devices:**

While dynamic allocation provides flexibility, it has important limitations on embedded devices:
- **Memory fragmentation**: Repeated allocations/deallocations can fragment limited RAM
- **No garbage collection**: Memory leaks can be critical on devices with KB of RAM
- **Unpredictable timing**: Allocation times are non-deterministic
- **Stack vs heap**: Many microcontrollers have very limited heap space

**Best Practices:**
- Pre-allocate Vector capacity when possible using `Reserve()`
- Avoid frequent additions/removals during runtime
- Create Updatable objects at startup rather than dynamically
- Monitor free memory on resource-constrained devices
- Consider static allocation for mission-critical applications

## Installation

Install this library through the Arduino Library Manager or manually. **No additional dependencies required.**

## Usage

### The Problem: Blocking Code

Traditional Arduino code blocks execution:

```cpp
void loop() {
  digitalWrite(LED1, HIGH);
  delay(1000);  // ❌ Everything stops here!
  digitalWrite(LED1, LOW);
  delay(1000);

  // Can't do other things while waiting
}
```

### The Solution: Non-blocking with Updatable

```cpp
#include <Updatable.h>

// Create a non-blocking LED blinker
class LEDBlinker : public Updatable {
private:
  int pin;
  unsigned long interval;
  unsigned long elapsed;
  bool state;

public:
  LEDBlinker(int ledPin, unsigned long blinkInterval)
    : pin(ledPin), interval(blinkInterval), elapsed(0), state(false) {
    pinMode(pin, OUTPUT);
  }

  void update(unsigned long deltaTime) override {
    elapsed += deltaTime;
    if (elapsed >= interval) {
      elapsed = 0;
      state = !state;
      digitalWrite(pin, state);
    }
  }
};

// Create multiple non-blocking components
LEDBlinker led1(13, 1000);  // Blink every 1 second
LEDBlinker led2(12, 500);   // Blink every 0.5 seconds
// Both run concurrently without blocking!

void setup() {
  // Components are automatically registered
}

void loop() {
  // Option 1: Automatic delta time (recommended, easiest!)
  Updatable::updateAllInstances();  // Calculates delta time automatically

  // Option 2: Manual delta time (if you need more control)

  // Updatable::updateAllInstances(deltaTime);

  // Your loop remains responsive!
  // Add more non-blocking code here
}
```

**Key Benefits:**
- ✅ Multiple LEDs blink at different rates simultaneously
- ✅ No `delay()` calls blocking your program
- ✅ Loop stays responsive for other tasks
- ✅ Clean, modular, object-oriented code
- ✅ Automatic delta time calculation with overflow protection
- ✅ Ultra-simple: just call `updateAllInstances()` in your loop!

## Real-World Use Case

A typical application is a control panel with buttons, LEDs, and network communication. The panel has:
- Buttons that need to be read and debounced
- LEDs that sometimes blink, sometimes stay on or off depending on system state
- A network layer processing queued packets in the background

With Updatable, each component (buttons, LED controllers, network packet processor) simply inherits from `Updatable` and implements its own `update()` logic. A fixed number of instances are created at startup. Then, a single `updateAllInstances()` call in the main loop takes care of updating everything without any blocking.

This keeps the code clean and modular. Each component handles its own timing internally using the provided `deltaTime`, without scattering `millis()` checks throughout your program.

## API Reference

### Constructor & Destructor
- `Updatable()` - Automatically registers the instance
- `~Updatable()` - Automatically deregisters the instance
  - **WARNING**: Never delete an `Updatable` object from within its own `update()` method or during `updateAllInstances()` execution, as this will modify the registry while it's being iterated, causing undefined behavior

### Virtual Methods
- `virtual void update(unsigned long deltaTime)` - Override this in derived classes to implement update logic

### Static Methods
- `static void updateAllInstances()` - Updates all registered instances with automatic delta time calculation (recommended)
  - Handles millis() overflow automatically
  - Note: This method is not thread-safe
- `static void updateAllInstances(unsigned long deltaTime)` - Updates all registered instances with manual delta time
- `static void setDebugMode(bool mode)` - Enables/disables debug mode for all instances

### Instance Methods
- `bool debugMode()` - Returns the current debug mode state

## License

This library is licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).

**What this means:**
- ✅ Free to use in personal and commercial projects
- ✅ You can modify and distribute the library
- ✅ Can be used in proprietary/closed-source applications
- ⚠️ If you modify the library itself, you must share your changes under LGPL-3.0
- ⚠️ Modified versions must be clearly marked as such

See [LICENSE](LICENSE) for full details.

### Why LGPL-3.0?

This license ensures:
1. The library remains free for everyone
2. Improvements benefit the entire community
3. You can still use it in commercial products without restrictions
4. The library can evolve with contributions from users

## Author

Francois Nouaille-Degorce
Copyright (C) 2012-2025
