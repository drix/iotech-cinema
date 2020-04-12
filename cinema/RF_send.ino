
#define RF_TRANSMITER 15 // D8

static const int SCREEN_CLOSING = 1;
static const int SCREEN_OPENING = 2;
static const int SCREEN_CLOSE = 3;
static const int SCREEN_OPEN = 4;
static const int SCREEN_HALT = 5;

void RF_send_setup(){
  pinMode(RF_TRANSMITER,OUTPUT);
}

void RF_send_trasmit(unsigned int * data, unsigned int signalLength, unsigned int repeat){
  RF_receiver_disable();
  int onoff = LOW;
  for (int j = 0; j < repeat; j++) {
    for (int i = 0; i < 132; i++) {
      digitalWrite(RF_TRANSMITER, onoff);
      delayMicroseconds(data[i] * signalLength);
      onoff = (onoff == LOW) ? HIGH : LOW;
    }
  }
  digitalWrite(RF_TRANSMITER, LOW);
  RF_receiver_enable();
}

void setScreenState(int state){
    switch(state) {
       case SCREEN_CLOSING  : Sonar_stop_detection();  break; 
       case SCREEN_OPENING  : Sonar_start_detection(); break; 
       case SCREEN_CLOSE    : Sonar_stop_detection();  break; 
       case SCREEN_OPEN     : Sonar_stop_detection();  break; 
       case SCREEN_HALT     : Sonar_stop_detection();  break; 
    }
}

bool RF_send(int action) {
    switch(action) {
    
       case 1  :
          if(Sonar_is_ON()){
            RF_send(SCREEN_HALT);
            delay(500);
          }
          setScreenState(SCREEN_CLOSING);
          RF_send_trasmit(Screen.up, Screen.signalLength, 10);
          Mosquito_log("close screen");
          break; /* optional */
      
       case -1 :
          setScreenState(SCREEN_OPENING);
          RF_send_trasmit(Screen.down, Screen.signalLength, 10);
          Mosquito_log("open screen");
          break; /* optional */
          
       case 0  :
          setScreenState(SCREEN_HALT);
          RF_send_trasmit(Screen.halt, Screen.signalLength, 10);
          Mosquito_log("halt screen");
          break; /* optional */
      
       /* you can have any number of case statements */
       default : /* Optional */
          RF_receiver_enable();
          return false;
    }
    return true;
}
