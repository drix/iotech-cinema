#include <math.h>
#define SAMPLESIZE 135

static int receiverPin = 0; // D3
static int interruptPin = 5; // D1
static int action = 0;
static long debounceTime = 0;
static bool debouncing = false;

void ICACHE_RAM_ATTR handleInterrupt ();

void RF_receiver_loop() {
  const long time = millis();
  if(action > 0 && !debouncing) {
      debounceTime = time + 1000;
      debouncing = true;
      RF_receiver_disable();
      int err;
      if(action == 1) {
        err = ewelink_toggle("room 1");
      } else
      if(action == 2) {
        IR_turn_on_projector();
      } else
      if(action == 3) {
        err = ewelink_toggle("room 2");
      } else
      if(action == 4) {
        setScreenState(1);
        IR_turn_off_projector();
      } else
      if(action == 5) {
        setScreenState(2);
        IR_turn_on_projector();
      } else
      if(action == 6) {
        setScreenState(5);
      }
      Mosquito_log("execute action:" + String(action));
   }
   action = 0;
   if(debouncing && time > debounceTime) {
      debouncing = false;
      RF_receiver_enable();
   }
}

void RF_receiver_enable() {
  resetSniffer();
  attachInterrupt(digitalPinToInterrupt(receiverPin), handleInterrupt, CHANGE);
}

void RF_receiver_disable() {
  resetSniffer();
  detachInterrupt(digitalPinToInterrupt(receiverPin));
}


static unsigned int bufferSniffer[SAMPLESIZE];
static unsigned int changesCount = 0;
static bool recording = false;
static long lastTime = 0;

void resetSniffer() {
  changesCount = 0;
  recording = false;
}

void readSignal(unsigned int normalizedBuffer[]) {
  if (memcmp(normalizedBuffer, Button.left1, sizeof(Button.left1)) == 0 || 
      memcmp(normalizedBuffer, Button.left2, sizeof(Button.left2)) == 0) 
  {
    action = 1;
  } else
  if (memcmp(normalizedBuffer, Button.middle1, sizeof(Button.middle1)) == 0 ||
      memcmp(normalizedBuffer, Button.middle2, sizeof(Button.middle2)) == 0)
  {
    action = 2;
  } else
  if (memcmp(normalizedBuffer, Button.right1, sizeof(Button.right1)) == 0 ||
      memcmp(normalizedBuffer, Button.right2, sizeof(Button.right2)) == 0)
  {
    action = 3;
  } else
  if (memcmp(normalizedBuffer, Screen.up, sizeof(Screen.up)) == 0)
  {
    action = 4;
  } else
  if (memcmp(normalizedBuffer, Screen.down, sizeof(Screen.down)) == 0)
  {
    action = 5;
  } else
  if (memcmp(normalizedBuffer, Screen.halt, sizeof(Screen.halt)) == 0)
  {
    action = 6;
  }
}

bool inRange(unsigned low, unsigned high, unsigned x) 
{ 
    return  ((x-low) <= (high-low)); 
} 

int getSignal(long elapsitime){
  if(inRange(270, 380, elapsitime) || inRange(160, 269, elapsitime)) return 1;
  if(inRange(800, 1060, elapsitime) || inRange(510, 610, elapsitime)) return 3;
  if(inRange(9820, 10080, elapsitime)) return 33;
  if(inRange(4940, 5140, elapsitime)) return 28;
  return 0;
}

void handleInterrupt() {
  const long time = micros();
  const long elapsitime = (time - lastTime);
  lastTime = time;

  // noise
  if(elapsitime < 120) {
    resetSniffer();
    return;
  }
  
  // sniff changes
  int currentSignal = getSignal(elapsitime);

  // not our protocol
  if(currentSignal == 0) {
    if (recording && WiFi_shouldlog()) {
      String msg = "stop recording with: "+ String(elapsitime) + " count: " + String(changesCount) + " raw: ";
      for (int i = 0; i < changesCount; i++) msg = msg + String(bufferSniffer[i]) + ",";
      Mosquito_log(msg);
    }
//    if(recording) Serial.println("stop recording with: "+ String(elapsitime));
    resetSniffer();
    return;
  }

  // save signal
  bufferSniffer[changesCount++] = currentSignal;

  // find connect signal
  if(!recording && (currentSignal == Button.signalLong || currentSignal == Screen.signalLong)) {
      recording = true;
      changesCount = 0;
      return;
  } else
  if(currentSignal == Button.signalLong || currentSignal == Screen.signalLong) {
      readSignal(bufferSniffer);
      if(action == 0){
        changesCount = 0;
      } else {
        resetSniffer();
      }
      return;
  } else
  if (changesCount > (SAMPLESIZE - 1)) {
      resetSniffer();
      return;
  }
}
