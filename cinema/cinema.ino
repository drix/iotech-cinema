/*
 * Adriano Spadoni 
 * 
 * ESP8266 to control:
 * - projector with alexa + IR 
 * - screen with alexa + RF
 * - sonoff switchs with RF + MQTT ?
 * - bed lights with alexa + RF 
 * - airco with alexa + RF
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ButtonClass.h"
#include "ScreenClass.h"

#define SERIAL_BAUDRATE 115200

void setup() {
  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();

  WiFi_setup();
  
  WiFi_server_setup();
  
  Mosquito_setup();

  fauxmo_setup();

  RF_send_setup();
  
  RF_receiver_enable();

  IR_setup();

  Sonar_setup();

  Mosquito_log("All system nominal ");
}

void loop() {
  WiFi_loop();

  Mosquito_loop();
  
  fauxmo_loop();

  RF_receiver_loop();
  
  Sonar_loop();

  yield();             // Feed the WDT (again)
}
