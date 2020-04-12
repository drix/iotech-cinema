// defines pins numbers

#define TRIGER_PIN 13 //D7
#define ECHO_PIN 14 //D5
#define SONAR_PIN 4 //D2

// defines variables
long duration;
int distance;

static bool shouldDetect = false;
static bool isSonarOn = false;
static long timeout_detection = millis();
static long last_detection = millis();
  
void Sonar_setup() {
  pinMode(TRIGER_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an Input
}


bool Sonar_is_ON(){
  return isSonarOn;
}

void Sonar_start_detection(){
    Mosquito_log("start detection");
    digitalWrite(SONAR_PIN, HIGH);
    timeout_detection = millis() + 60000;
    shouldDetect = true;
    isSonarOn = true;
}

void Sonar_stop_detection(){
    digitalWrite(SONAR_PIN, LOW);
    shouldDetect = false;
    isSonarOn = false;
    Mosquito_log("stop detection");
}
void Sonar_loop() {
  long milliseconds = millis();
  if(!shouldDetect || milliseconds < last_detection) return;

  // Clears the trigPin
  digitalWrite(TRIGER_PIN, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGER_PIN, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculating the distance
  distance = duration*0.034/2;

  if((distance > 0 && distance < 30) || milliseconds > timeout_detection) {
    Mosquito_log("found obstacle:" + String(distance));
    RF_send(0);
  } 
  last_detection = milliseconds + 1000;
}
