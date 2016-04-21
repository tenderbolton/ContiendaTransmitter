

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile


RH_ASK driver;

int currentState = 0;
String incomingString;
unsigned long stateTime = 0;

void emitState(){
  
  const char *OFFstate = "OFF";
  const char *ONstate = "ON";

  digitalWrite(13, false); // Flash a light to show transmitting
  if(currentState == 0){
    //vw_send((uint8_t *)OFFstate, strlen(OFFstate));
    driver.send((uint8_t *)OFFstate, strlen(OFFstate));
    
    Serial.println("OFF");
  }
  else{
    //vw_send((uint8_t *)ONstate, strlen(ONstate));
    driver.send((uint8_t *)ONstate, strlen(ONstate));
    
    Serial.println("ON");
  }
  driver.waitPacketSent();
  digitalWrite(13, true);
}

void processInput(String inputData){
  
    if(inputData=="ON\n"){
      currentState = 1;
      stateTime = millis();
      //we sleep 1 second until next ON message
    }
    if(inputData=="OFF\n"){
      currentState = 0;
      stateTime = millis();
      //we sleep 1 second until next ON message
    }
}

void processSerialInput(){
  // Check if there's incoming serial data.
  if (Serial.available() > 0) {
    // Read a byte from the serial buffer.
    char incomingByte = (char)Serial.read();
    incomingString += incomingByte;

    // Checks for null termination of the string.
    if (incomingByte == '\n') {
      // ...do something with String...
      Serial.println(incomingString);
      processInput(incomingString);
      incomingString = "";
    }
  }
}

void setup()
{
    pinMode(13, OUTPUT);
    
    Serial.begin(9600);	  // Debugging only
    Serial.println("setup");

}

void loop()
{
    processSerialInput();
    emitState();
    if(currentState == 1){
      if(millis()>stateTime+10000){
        currentState = 0;
      }
    }
    //delay(1000);
    
}
