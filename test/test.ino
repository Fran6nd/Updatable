/*
 * Updatable Library Test Suite
 *
 * Upload this sketch to your Arduino board to run tests.
 * Open Serial Monitor at 9600 baud to see results.
 */

#include <Updatable.h>

// Test counter class
class TestCounter : public Updatable {
public:
  int updateCount;
  unsigned long totalDelta;

  TestCounter() : updateCount(0), totalDelta(0) {}

  void update(unsigned long deltaTime) override {
    updateCount++;
    totalDelta += deltaTime;
  }

  void reset() {
    updateCount = 0;
    totalDelta = 0;
  }
};

// Test variables
int testsPassed = 0;
int testsFailed = 0;

void testAssert(const char* testName, bool condition) {
  Serial.print("Test: ");
  Serial.print(testName);
  Serial.print(" ... ");

  if (condition) {
    Serial.println("PASS");
    testsPassed++;
  } else {
    Serial.println("FAIL");
    testsFailed++;
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for some boards)
  }

  delay(1000);

  Serial.println("========================================");
  Serial.println("Updatable Library Test Suite");
  Serial.println("========================================");
  Serial.println();

  runTests();

  Serial.println();
  Serial.println("========================================");
  Serial.println("Test Results:");
  Serial.print("  Passed: ");
  Serial.println(testsPassed);
  Serial.print("  Failed: ");
  Serial.println(testsFailed);
  Serial.println("========================================");

  if (testsFailed == 0) {
    Serial.println("ALL TESTS PASSED!");
  } else {
    Serial.println("SOME TESTS FAILED!");
  }
}

void runTests() {
  // Test 1: Instance Registration
  Serial.println("--- Test 1: Instance Registration ---");
  {
    TestCounter* counter1 = new TestCounter();
    TestCounter* counter2 = new TestCounter();

    Updatable::updateAllInstances(100);

    testAssert("Counter 1 received update", counter1->updateCount == 1);
    testAssert("Counter 2 received update", counter2->updateCount == 1);
    testAssert("Counter 1 received correct deltaTime", counter1->totalDelta == 100);
    testAssert("Counter 2 received correct deltaTime", counter2->totalDelta == 100);

    delete counter1;
    delete counter2;
  }
  Serial.println();

  // Test 2: Multiple Updates
  Serial.println("--- Test 2: Multiple Updates ---");
  {
    TestCounter* counter = new TestCounter();

    Updatable::updateAllInstances(50);
    Updatable::updateAllInstances(75);
    Updatable::updateAllInstances(25);

    testAssert("Counter received 3 updates", counter->updateCount == 3);
    testAssert("Counter accumulated deltaTime correctly", counter->totalDelta == 150);

    delete counter;
  }
  Serial.println();

  // Test 3: Debug Mode
  Serial.println("--- Test 3: Debug Mode ---");
  {
    TestCounter* counter1 = new TestCounter();
    TestCounter* counter2 = new TestCounter();

    testAssert("Debug mode off by default (counter1)", !counter1->debugMode());
    testAssert("Debug mode off by default (counter2)", !counter2->debugMode());

    Updatable::setDebugMode(true);

    testAssert("Debug mode enabled (counter1)", counter1->debugMode());
    testAssert("Debug mode enabled (counter2)", counter2->debugMode());

    Updatable::setDebugMode(false);

    testAssert("Debug mode disabled (counter1)", !counter1->debugMode());
    testAssert("Debug mode disabled (counter2)", !counter2->debugMode());

    delete counter1;
    delete counter2;
  }
  Serial.println();

  // Test 4: Virtual Method Override
  Serial.println("--- Test 4: Virtual Method Override ---");
  {
    class CustomUpdatable : public Updatable {
    public:
      bool wasUpdated;
      CustomUpdatable() : wasUpdated(false) {}
      void update(unsigned long deltaTime) override {
        wasUpdated = true;
      }
    };

    CustomUpdatable* custom = new CustomUpdatable();

    testAssert("Custom update not called yet", !custom->wasUpdated);

    Updatable::updateAllInstances(100);

    testAssert("Custom update was called", custom->wasUpdated);

    delete custom;
  }
  Serial.println();

  // Test 5: Zero Delta Time
  Serial.println("--- Test 5: Zero Delta Time ---");
  {
    TestCounter* counter = new TestCounter();

    Updatable::updateAllInstances(0);

    testAssert("Update called with zero delta", counter->updateCount == 1);
    testAssert("Zero delta accumulated", counter->totalDelta == 0);

    delete counter;
  }
  Serial.println();

  // Test 6: Destructor Cleanup
  Serial.println("--- Test 6: Destructor Cleanup ---");
  {
    TestCounter* counter1 = new TestCounter();
    TestCounter* counter2 = new TestCounter();

    Updatable::updateAllInstances(100);

    testAssert("Both counters received first update",
               counter1->updateCount == 1 && counter2->updateCount == 1);

    delete counter1;  // Remove first counter
    counter1 = nullptr;

    counter2->reset();
    Updatable::updateAllInstances(100);

    testAssert("Only counter2 received second update", counter2->updateCount == 1);
    testAssert("Counter2 still works after counter1 deleted", counter2->totalDelta == 100);

    delete counter2;
  }
  Serial.println();
}

void loop() {
  // Nothing to do in loop
  delay(1000);
}
