#pragma once

#include <Arduino.h>

class WebConnector {
  public:
    // Initialize the server
    void init();
    void handleConnection();
    String getActiveCommand();
  private:
    String input;
    String activeCommand;

};