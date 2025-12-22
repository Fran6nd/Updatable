/*
 * Updatable Library - Non-blocking Programming Framework for Arduino
 * Copyright (C) 2012-2025 Francois Nouaille-Degorce
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef UPDATABLE_H
#define UPDATABLE_H

#include <Arduino.h>
#include <Vector.h>

class Updatable {
public:
  Updatable() {
    debug = false;  // Ensure debug is off by default
    registeredInstances.PushBack(this);
  }

  virtual ~Updatable() {
    // Remove this instance from the registry to prevent dangling pointers
    // WARNING: Never delete an Updatable object from within its own update()
    // method or during updateAllInstances() execution, as this will modify
    // the registry while it's being iterated, causing undefined behavior.
    for (int i = 0; i < registeredInstances.Size(); i++) {
      if (registeredInstances[i] == this) {
        registeredInstances.Erase(i);
        break;
      }
    }
  }

  // Override this function in derived classes to provide custom update logic.
  virtual void update(unsigned long deltaTime) {
    // Default implementation does nothing. To be overridden.
  }

  // Update all registered instances with automatic delta time calculation.
  // Handles millis() overflow automatically.
  // Note: This method is not thread-safe.
  static void updateAllInstances() {
    unsigned long now = millis();

    if (firstCall) {
      firstCall = false;
      lastUpdate = now;
      return;
    }

    unsigned long delta = now - lastUpdate;
    lastUpdate = now;

    int count = registeredInstances.Size();
    for (int i = 0; i < count; i++) {
      registeredInstances[i]->update(delta);
    }
  }

  // Update all registered instances with the given time delta.
  static void updateAllInstances(unsigned long deltaTime) {
    int count = registeredInstances.Size();
    for (int i = 0; i < count; i++) {
      registeredInstances[i]->update(deltaTime);
    }
  }

  // Set all registered instances into debug mode.
  static void setDebugMode(bool mode) {
    int count = registeredInstances.Size();
    for (int i = 0; i < count; i++) {
      registeredInstances[i]->_setDebugMode(mode);
    }
  }

  bool debugMode(){
    return debug;
  }


private:
  // Store pointers to all registered instances for updating.
  static Vector<Updatable *> registeredInstances;
  static unsigned long lastUpdate;
  static bool firstCall;
  bool debug;
  // Used to propagate the debug mode. Internal use only.
  void _setDebugMode(bool mode){
    debug = mode;
  }
};

// Initialize the static members outside the class definition.
Vector<Updatable *> Updatable::registeredInstances;
unsigned long Updatable::lastUpdate = 0;
bool Updatable::firstCall = true;

#endif
