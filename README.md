# Updatable

An Arduino library providing a base class for objects that need periodic updates.

## Description

The Updatable library simplifies managing multiple time-dependent objects in Arduino projects. By inheriting from the `Updatable` class, your objects are automatically registered and can be updated in batch with a single function call.

## Features

- Automatic registration of updatable objects
- Batch update all registered instances
- Debug mode support for all instances
- Minimal overhead and easy integration

## Dependencies

This library requires the [Vector library by Janelia](https://github.com/janelia-arduino/Vector) for managing the collection of registered instances.

## Installation

1. Install this library through the Arduino Library Manager or manually
2. Install the Vector library dependency from the Arduino Library Manager

## Usage

```cpp
#include <Updatable.h>

class MyComponent : public Updatable {
public:
  void update(long int deltaTime) override {
    // Your update logic here
    // deltaTime is in milliseconds
  }
};

MyComponent component1;
MyComponent component2;

void setup() {
  // Components are automatically registered
}

void loop() {
  static unsigned long lastUpdate = 0;
  unsigned long now = millis();
  long int deltaTime = now - lastUpdate;
  lastUpdate = now;

  // Update all registered Updatable instances
  Updatable::updateAllInstances(deltaTime);

  delay(10);
}
```

## API Reference

### Constructor
- `Updatable()` - Automatically registers the instance

### Virtual Methods
- `virtual void update(long int deltaTime)` - Override this in derived classes to implement update logic

### Static Methods
- `static void updateAllInstances(long int deltaTime)` - Updates all registered instances
- `static void setDebugMode(bool mode)` - Enables/disables debug mode for all instances

### Instance Methods
- `bool DebugMode()` - Returns the current debug mode state

## License

This library is provided as-is for use in Arduino projects.

## Author

Original author: tom
