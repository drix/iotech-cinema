
#include "fauxmoESP.h"

#define SCREEN "cinema"
#define PROJECTOR "projector"

static fauxmoESP fauxmo;

bool triggerProjectorON = false;
bool triggerProjectorOFF = false;
bool triggerScreenON = false;
bool triggerScreenOFF = false;

void fauxmo_loop(){
  // fauxmoESP uses an async TCP server but a sync UDP server
  // Therefore, we have to manually poll for UDP packets
  fauxmo.handle();

  if(triggerProjectorON) {
    triggerProjectorON = false;
    IR_turn_on_projector();
  }
  
  if(triggerProjectorOFF) {
    triggerProjectorOFF = false;
    IR_turn_off_projector();
  }
  
  if(triggerScreenON) {
    triggerScreenON = false;
    RF_send(-1);
  }
  
  if(triggerScreenOFF) {
    triggerScreenOFF = false;
    RF_send(1);
  }
}

void fauxmo_setup(){
  // You can use different ways to invoke alexa to modify the devices state:
  // Add virtual devices
  fauxmo.addDevice(SCREEN);
  fauxmo.addDevice(PROJECTOR);
  fauxmo.setPort(80); // This is required for gen3 devices
  fauxmo.enable(true);
  

  Mosquito_log("added SCREEN & PROJECTOR");
    // If your device state is changed by any other means (MQTT, physical button,...)
    // you can instruct the library to report the new state to Alexa on next request:
    // fauxmo.setState(ID_YELLOW, true, 255);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    // Callback when a command from Alexa is received. 
    // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
    // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
    // Just remember not to delay too much here, this is a callback, exit as soon as possible.
    // If you have to do something more involved here set a flag and process it in your main loop.
        
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if ( (strcmp(device_name, SCREEN) == 0) ) {
      // this just sets a variable that the main loop() does something about
      if (state) {
        triggerScreenON = true;
      } else {
        triggerScreenOFF = true;
      }
      Mosquito_log("Screen switched by Alexa");
    }
    if ( (strcmp(device_name, PROJECTOR) == 0) ) {
      // this just sets a variable that the main loop() does something about
      if (state) {
        triggerProjectorON = true;
      } else {
        triggerProjectorOFF = true;
      }
      Mosquito_log("Projector switched by Alexa");
    }
  });
}
