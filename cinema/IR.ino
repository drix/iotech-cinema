#include <IRremoteESP8266.h>
#include <IRsend.h>

#define IR_RECEIVE_PIN 16 // D0
#define IR_SEND_PIN 5 // D1

IRsend irsend(IR_SEND_PIN);  // Set the GPIO to be used to sending the message.

void IR_setup()
{
  pinMode(IR_RECEIVE_PIN, INPUT);
  irsend.begin();
}

void IR_turn_on_projector(){
  RF_receiver_disable();
  irsend.sendNEC(0xCF20D, 32);
  RF_receiver_enable();
}

void IR_turn_off_projector(){
  RF_receiver_disable();
  irsend.sendNEC(0xC728D, 32);
  delay(2000);
  irsend.sendNEC(0xC728D, 32);
  delay(2000);
  irsend.sendNEC(0xC728D, 32);
  delay(2000);
  irsend.sendNEC(0xC728D, 32);
  RF_receiver_enable();
}
