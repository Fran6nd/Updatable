# Testing Guide for Updatable Library

This document describes how to test the Updatable library using different methods.

## Method 1: Manual Testing on Arduino Hardware

### Prerequisites
- Arduino IDE installed
- Vector library installed (via Library Manager)
- Arduino board (any compatible board)

### Steps
1. Open Arduino IDE
2. Open the test sketch: `test/test.ino`
3. Select your Arduino board in Tools > Board
4. Select the correct port in Tools > Port
5. Upload the sketch to your board
6. Open Serial Monitor (Tools > Serial Monitor)
7. Set baud rate to 9600
8. View test results

### Expected Output
```
========================================
Updatable Library Test Suite
========================================

--- Test 1: Instance Registration ---
Test: Counter 1 received update ... PASS
Test: Counter 2 received update ... PASS
...

========================================
Test Results:
  Passed: 13
  Failed: 0
========================================
ALL TESTS PASSED!
```

## Method 2: Arduino CLI (Command Line)

### Prerequisites
```bash
# Install Arduino CLI
brew install arduino-cli  # macOS
# or download from: https://arduino.github.io/arduino-cli/

# Update core index
arduino-cli core update-index

# Install AVR core
arduino-cli core install arduino:avr

# Install Vector library
arduino-cli lib install Vector
```

### Compile Tests
```bash
# Compile test sketch
arduino-cli compile --fqbn arduino:avr:uno test/test.ino

# Compile example
arduino-cli compile --fqbn arduino:avr:uno examples/BasicUsage/BasicUsage.ino
```

### Upload and Monitor
```bash
# Upload test sketch
arduino-cli upload -p /dev/cu.usbmodem14101 --fqbn arduino:avr:uno test/test.ino

# Monitor serial output
arduino-cli monitor -p /dev/cu.usbmodem14101 -c baudrate=9600
```

## Method 3: Automated CI Testing (GitHub Actions)

The library includes GitHub Actions workflow for automated testing.

### What It Tests
- Compilation on multiple Arduino boards:
  - Arduino UNO (AVR)
  - Arduino MEGA (AVR)
  - Arduino MKR1000 (SAMD)
  - ESP32
  - ESP8266
- Both test sketch and examples
- Automatic dependency installation

### Setup
1. Push your code to GitHub
2. The workflow runs automatically on push/PR
3. View results in GitHub Actions tab

### Manual Trigger
```bash
# Push to trigger
git push origin main

# Or use GitHub UI:
# Go to Actions tab > Arduino CI > Run workflow
```

## Method 4: PlatformIO Testing (Advanced)

### Prerequisites
```bash
# Install PlatformIO
pip install platformio
```

### Create platformio.ini
```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino
lib_deps =
    janelia-arduino/Vector

[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps =
    janelia-arduino/Vector
```

### Run Tests
```bash
# Test on UNO
pio ci --board=uno test/test.ino

# Test on ESP32
pio ci --board=esp32dev test/test.ino
```

## Test Coverage

The test suite ([test/test.ino](test/test.ino)) covers:

1. **Instance Registration** - Verifies objects are automatically registered
2. **Multiple Updates** - Tests repeated update calls and delta accumulation
3. **Debug Mode** - Tests debug mode enable/disable for all instances
4. **Virtual Method Override** - Ensures derived classes can override update()
5. **Zero Delta Time** - Tests edge case with zero time delta

## Continuous Integration

The library uses GitHub Actions for CI/CD:
- [.github/workflows/arduino-ci.yml](.github/workflows/arduino-ci.yml)
- Runs on every push and pull request
- Tests multiple board architectures
- Automatically installs dependencies

## Adding New Tests

To add new tests to [test/test.ino](test/test.ino):

1. Create a new test function in `runTests()`
2. Use the `assert()` helper function
3. Follow the existing pattern:

```cpp
Serial.println("--- Test X: Description ---");
{
  // Test setup
  TestCounter* counter = new TestCounter();

  // Test action
  Updatable::updateAllInstances(100);

  // Test assertion
  assert("Test description", counter->updateCount == 1);

  // Cleanup
  delete counter;
}
Serial.println();
```

## Troubleshooting

### Vector library not found
```bash
# Manually install Vector library
arduino-cli lib install Vector
```

### Compilation errors
- Ensure you're using Arduino IDE 1.8.13 or later
- Check that Vector library is installed
- Verify board core is installed

### Serial Monitor shows nothing
- Check baud rate is set to 9600
- Try resetting the board
- Verify USB connection and port selection
