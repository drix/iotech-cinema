//#include <ESP8266WiFi.h>
//#include "Adafruit_MQTT.h"
//#include "Adafruit_MQTT_Client.h"
//
///************************* Adafruit.io Setup *********************************/
//
//#define AIO_SERVER      "io.adafruit.com"
//#define AIO_SERVERPORT  1883                   // use 8883 for SSL
//#define IO_USERNAME     "drix"
//#define IO_KEY          "***"
//
///************ Global State (you don't need to change this!) ******************/
//
//// Create an ESP8266 WiFiClient class to connect to the MQTT server.
//WiFiClient client;
//// or... use WiFiFlientSecure for SSL
////WiFiClientSecure client;
//
//// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
//Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, IO_USERNAME, IO_KEY);
//
///****************************** Feeds ***************************************/
//
//// Setup a feed called 'room_light_1' for publishing.
//// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
//Adafruit_MQTT_Publish room_light_1 = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/room_light_1");
////Adafruit_MQTT_Publish room_light_2 = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/room_light_2");
//
//// Setup a feed called 'onoff' for subscribing to changes.
////Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/room");
//
//bool MQTT_Publish_room_light_1(String msg) {
//  MQTT_connect();
//  Serial.println("MQTT_Publish_room_light_1: " + String(msg));
//  // Length (with one extra character for the null terminator)
//  int str_len = msg.length() + 1; 
//  // Prepare the character array (the buffer) 
//  char char_array[str_len];
//  // Copy it over 
//  msg.toCharArray(char_array, str_len);
//  if(! room_light_1.publish(char_array)){
//    Serial.println("Error MQTT_Publish_room_light_1");
//    return false;
//   } 
//   Serial.println("Published!");
//   return true;
//}
//
///*************************** Sketch Code ************************************/
//
//// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
//// for some reason (only affects ESP8266, likely an arduino-builder bug).
//void MQTT_loop() {
//  // Ensure the connection to the MQTT server is alive (this will make the first
//  // connection and automatically reconnect when disconnected).  See the MQTT_connect
//  // function definition further below.
////  MQTT_connect();
//
//  // this is our 'wait for incoming subscription packets' busy subloop
//  // try to spend your time here
//
////  Adafruit_MQTT_Subscribe *subscription;
////  while ((subscription = mqtt.readSubscription(5000))) {
////    if (subscription == &onoffbutton) {
////      Serial.print(F("Got: "));
////      Serial.println((char *)onoffbutton.lastread);
////    }
////  }
//
////  // Now we can publish stuff!
////  Serial.print(F("\nSending photocell val "));
////  Serial.print(x);
////  Serial.print("...");
////  if (! photocell.publish(x++)) {
////    Serial.println(F("Failed"));
////  } else {
////    Serial.println(F("OK!"));
////  }
//
//  // ping the server to keep the mqtt connection alive
//  // NOT required if you are publishing once every KEEPALIVE seconds
//  /*
//  if(! mqtt.ping()) {
//    mqtt.disconnect();
//  }
//  */
//}
//
//// Function to connect and reconnect as necessary to the MQTT server.
//// Should be called in the loop function and it will take care if connecting.
//void MQTT_connect() {
//  int8_t ret;
//
//  // Stop if already connected.
//  if (mqtt.connected()) {
//    return;
//  }
//
//  Serial.print("Connecting to MQTT... ");
//
//  uint8_t retries = 3;
//  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
//       Serial.println(mqtt.connectErrorString(ret));
//       Serial.println("Retrying MQTT connection in 5 seconds...");
//       mqtt.disconnect();
//       delay(5000);  // wait 5 seconds
//       retries--;
//       if (retries == 0) {
//         // basically die and wait for WDT to reset me
//         while (1);
//       }
//  }
//  Serial.println("MQTT Connected!");
//}
