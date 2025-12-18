#ifndef UPDATABLE_H
#define UPDATABLE_H

#include <Vector.h>

class Updatable {
public:
  Updatable() {
    this->debug = false;  // 🔹 Ensure debug is off by default
    Updatable::registeredInstances.push_back(this);
  }


  // Override this function in derived classes to provide custom update logic.
  virtual void update(long int deltaTime) {
    // Default implementation does nothing. To be overridden.
  }

  // Update all registered instances with the given time delta.
  static void updateAllInstances(long int deltaTime) {
    for (int i = 0; i < Updatable::registeredInstances.size(); i++) {
      Updatable::registeredInstances[i]->update(deltaTime);
    }
  }

  // Set all registered instances into debug mode.
  static void setDebugMode(bool mode) {
    for (int i = 0; i < Updatable::registeredInstances.size(); i++) {
      Updatable::registeredInstances[i]->_setDebugMode(mode);
    }
  }

  bool DebugMode(){
    return this->debug;
  }


private:
  // Store pointers to all registered instances for updating.
  static Vector<Updatable *> registeredInstances;
  bool debug;
  // Used to propagate the debug mode. Internal use only.
  void _setDebugMode(bool mode){
    this->debug = mode;
  }
};

// Initialize the static member outside the class definition.
Vector<Updatable *> Updatable::registeredInstances;

#endif
