#include <dht.h>
/* Project Name: Prototype Body Sensors
 * Author: Gemaris Velazquez
 * File name: BodySensor-Proto_GV.ino
 * 
 * Purpose of program: This program can currently record the
 * temperature and sweat. If a certain level of sweat
 * has been reached [the water sensor is drenched] or if there
 * is a certain level of heat reached [the temperature sensor
 * is high enough], then the light will go off to alert the 
 * user to take a break.
 * 
 * Parts currently used:
 * -FC-37 Rain Sensor
 * -KY-016 3-color LED module
 * -Buzzer
 * -DHT11 Humidity and Temperature Sensor
 * -Arduino Nano
 * -Breadboard
 * -3 AAA Battery Compartment
 * 
 * Future parts to be implemented:
 * -Body temperature sensor, to replace DHT sensor
 * -Glucose level sensor
 * -Oxygen level sensor
 * -Heartbeat sensor
 */

//Declare the variables, set them to their appropriate pins
dht DHT; //For interpreting DHT sensor
int DHTPin = 13; //Digital pin for DHT sensor, set pin to D13
int DHTRead; //Reads input from DHT sensor

int LEDPin = 2; //Digital pin for LED light, set pin to D2
int buzzerPin = 3; //Digital pin for buzzer, set pin to D3

const int SENS_MIN = 0; //Limits for reading sweat sensor
const int SENS_MAX = 1024;
int sweatPin = A0; //Analog pin for sweat sensor, set pin to A0
int sweatRead; //Reads input from sweat sensor

void setup() 
{
  //Set LED pin for output
  pinMode(LEDPin, OUTPUT);
  Serial.begin(9600); 
  Serial.println("Starting up sensors...");
}//End of setup function

void loop() 
{
  //Read in the sweat and DHT sensor for their inputs
  DHTRead = DHT.read11(DHTPin);
  sweatRead = analogRead(sweatPin);
  
  //If temperature too low, continue loop
  //Create sweatRead restriction for sweat sensor to limit itself
  //TODO: Find percentage that is considered "too sweaty", default: 100
  sweatRead = 99-map(sweatRead, SENS_MIN, SENS_MAX, 0, 100);
  
  //Output temperature and sweat level to serial monitor
  Serial.print("Temperature: ");
  Serial.println(DHT.temperature);
  Serial.print("Sweat level: ");
  Serial.print(sweatRead);

  //Reduce the sweatRead to 4 cases: drenched in sweat, warning of being drenched, normal amount of sweat, and no sweating
  if (sweatRead > 75)
    sweatRead = 3;
  else if (sweatRead > 50)
    sweatRead = 2;
  else if (sweatRead > 0)
    sweatRead = 1;
  
  //If both temperature and sweat levels are high, flash the light
  //Temperature evaluation
  if(DHT.temperature > 30)
  {
    //Sweat evaluation
    switch(sweatRead)
    {
      //Drenched case
      case 3:
        Serial.println(" - Very High");
        digitalWrite(LEDPin, HIGH);
        tone(buzzerPin, 1000);
        delay(1000);
        break;
      //Warning case   
      case 2:
        Serial.println(" - High");
        digitalWrite(LEDPin, HIGH);
        tone(buzzerPin, 500);
        delay(300);
        digitalWrite(LEDPin, LOW);
        noTone(buzzerPin);
        delay(700);
        break;
      //Normal case  
      case 1:
        Serial.println(" - Normal");
        delay(1000);
        break;
     //No sweat case   
      case 0:
        Serial.println(" - Not Sweating");
        delay(1000);
        break;   
    }//End of switch statement
  }//End of if human temperature statement

  //Turn LED and buzzer off
  digitalWrite(LEDPin, LOW);
  noTone(buzzerPin);
  
  //Add 2 new line statements to prepare the monitor written for the next prompt
  Serial.print("\n \n");
  
  //Delay 1 second
  delay(1000);
}//End of loop function
