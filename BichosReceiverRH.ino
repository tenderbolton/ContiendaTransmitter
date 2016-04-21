#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver;

#define RELAY1  6 

int currentState = 0;
unsigned long stateTime = 0;

void setup()
{
  
    pinMode(RELAY1, OUTPUT); 
    pinMode(13, OUTPUT); 
    
    digitalWrite(RELAY1,LOW);   // Turns Relay Off
    
    Serial.begin(9600);	// Debugging only
    Serial.println("setup");
   
}

void loop()
{
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
   
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
	int i;

	String received = "";

	for (i = 0; i < buflen; i++)
	{
          char c = (char) buf[i];
          received = received + c;
	}

        Serial.println(received);
        
        if(received == "OFF"){
          digitalWrite(RELAY1,LOW);
          currentState = 0;
        }
        
        if(received == "ON"){
          digitalWrite(RELAY1,HIGH);
          currentState = 1;
          stateTime = millis();
        }
        
        //digitalWrite(RELAY1,HIGH);          // Turns Relay Off
        //delay(1000);
        //digitalWrite(RELAY1,LOW);          // Turns Relay Off

    }
    
    //checking timeout for turning off
    if(currentState == 1){
      if(millis()>stateTime+10000){
        digitalWrite(RELAY1,LOW);
        currentState = 0;
      }
    }
}
