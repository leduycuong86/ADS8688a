/*  ADS8688 library example
 *   
 *  PIN CONNECTION: 
 *  
 *  REFSEL: GND (enable internal 4.096V reference)
 *  RST/PD: +5V/+3.3V
 *  AVDD:   +5V/+3.3V
 *  DVDD:   +5V/+3.3V
 *  CS:     pin 10 / MEGA pin 10
 *  MOSI:   pin 11 / MEGA pin 51
 *  MISO:   pin 12 / MEGA pin 50
 *  SCK:    pin 13 / MEGA pin 52
 *  REFIO:  NC
 *  ALARM:  NC
 *  DAISY:  NC
 */

#include "ADS8688.h"
#include <TimerOne.h>

#define TIMER_LOOP_1_SEC    1000000
#define BUFFER_SIZE         200
#define CHANNEL_NUM         8

float         ret[CHANNEL_NUM][BUFFER_SIZE] = {0};
unsigned long currentTime[CHANNEL_NUM];
uint16_t      cnt = 0;

ADS8688 bank = ADS8688();               // Instantiate ADS8688 with PIN 10 as default CS

void setup() {
  bank.setChannelSPD(0b11111111);
  bank.setGlobalRange(R0);              // set range for all channels
  //bank.autoRst();                       // reset auto sequence
  bank.manualChannel(0);
  bank.manualChannel(1);
  bank.manualChannel(2);
  bank.manualChannel(3);
  bank.manualChannel(4);
  bank.manualChannel(5);
  bank.manualChannel(6);
  bank.manualChannel(7);
  Serial.begin(115200);                 // start serial communication

  // Set up Timer1 with a period of 1000000 microseconds (1 second)
  Timer1.initialize(TIMER_LOOP_1_SEC);
  // Attach the function to be called when the timer fires
  Timer1.attachInterrupt(timerCallback);
  }

void loop() {
  
  for (int i=0;i<CHANNEL_NUM;i++)
  {  
    Serial.print("[ "); 
    for(int j = 0; j<BUFFER_SIZE; j++)
    { 
      Serial.print(ret[i][j]);
      Serial.print(", ");
    }
    Serial.println(" ]");  // print Volt label
  }
  currentTime[0] = millis(); 
  Serial.println(currentTime[0]);
  delay(1000); 
  
  //Serial.println("values: ");             // print label
#if 0
  for (byte i=0;i<8;i++) {
    uint16_t val = bank.noOp();         // trigger samples
    ret[i] = bank.I2V(val,R0);  
    //currentTime[i] = millis(); 
  }
  Serial.print("[ ");
  for (int i=0;i<8;i++)
  {    
    Serial.print(ret[i]);
    Serial.print(", ");
  }
  Serial.println(" ]");  // print Volt label
  #if 0
  for (int i=0;i<8;i++)
  {
    Serial.print(currentTime[i]);
    Serial.print(": CH");
    Serial.print(i);
    Serial.print(":");
    Serial.print(ret[i]);     // print value in Volts
    Serial.println(i!=7?" V | ":" V");  // print Volt label
  }
  #endif
  delay(1000);                           // wait for 500 milliseconds
#endif
  }

void timerCallback() {
  // Your code to run at the specified interval
  //Serial.println("One second has passed!");
  for(int k = 0; k<BUFFER_SIZE; k++)
  {
    for (byte i=0;i<CHANNEL_NUM;i++) {
      uint16_t val = bank.noOp();         // trigger samples
      ret[i][k] = bank.I2V(val,R0);  
    }
    delayMicroseconds(1);
  }
  //currentTime[0] = millis(); 
  //Serial.println(currentTime[0]);
  cnt++;
  if(cnt == BUFFER_SIZE)
    cnt = 0;
}
