
// #include <ESP8266WiFi.h>
// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <Servo.h>
#include "octosnake.h"
#include "minikame.h"
#include "webconnector.h"
#include "commandexecutor.h"

MiniKame robot;
WebConnector connector;
CommandExecutor executor;

void setup() {
  connector.init();
  robot.init();
  executor.init(&robot);
}

void loop() {
  // if there is a connection waiting, process it
  connector.handleConnection();
  
  // get the active command
  String activeCommand = connector.getActiveCommand();

  // execute the active command, which calls the robot
  executor.parseCommand(activeCommand);
}
