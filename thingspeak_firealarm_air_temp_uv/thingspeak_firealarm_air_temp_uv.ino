#include <SoftwareSerial.h>
#include "DHT.h"

#define RX 3 // Connected to Digital Pin 0 of Arduino   TX of Wifi connected to RX of Arduino
#define TX 2 // Connected to Digital Pin 1 of Arduino   RX of Wifi conneceted to TX of Arduino

String AP = "IIITK";       // Your Access Point Name
String PASS = "iiitkwlan";     // Your Password

/* 
 *  Here we are sending data to ThingSpeak API. 
 *  Create a free account in ThingSpeak.
 *  Then create a new channel. 
 *  Copy the API key for your new channel to below API String
 *  You can later browse the channel visualization to view the data
*/

dht DHT;
String API = "N9CPD6BNJ44OVCEV";   // ThingSpeak API Key
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1 = "field1";
String field2 = "field2";     //let it remain as field1 field2 etc...even if the graph name is different
String field3 = "field3";
String field4 = "field4";
String field5 = "field5";
String field7 = "field7";

const int buzzer = 8;
const int trigPin = 6;
const int echoPin = 7;
const int temp_sensor = A2;
const int air_sensor = A3;
const int flame_sensor = A4 ;
const int smoke_sensor = A5;

int smokeThres = 400;
int flameThres = 500;
int flame_detected;
int smoke_detected;
int air_quality;
long duration;
double distance;

int flame()
{
  flame_detected = analogRead(flame_sensor);
  return flame_detected;
}

int smoke()
{
   smoke_detected = analogRead(smoke_sensor);
   return smoke_detected;
}

int air()
{
  air_quality = analogRead(air_sensor);
  return air_quality;
}

int uv()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);                // Sets the trigPin on HIGH state for 10 micro seconds  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);          // Reads the echoPin, returns the sound wave travel time in microseconds  
  duration = pulseIn(echoPin, HIGH);   // Calculating the distance  
  distance= duration/58;          // Prints the distance on the Serial Monitor  
  return distance;
}

SoftwareSerial esp8266(RX,TX); // Initializing esp8266 package with proper pins
 
void setup() 
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(flame_sensor, INPUT) ;
  pinMode(buzzer, OUTPUT);
  pinMode(smoke_sensor, INPUT);
  Serial.begin(115200);
  esp8266.begin(115200);    // Setting the baud rate
  esp8266.println("AT");    // Sending AT command
  delay(1500);              // Delays are added between each esp8266 serial print for giving time to complete the respective command.
  esp8266.println("AT+CWMODE=1");   // Command to choose the Mode: 1 - Station Mode(client), 2 - AP Mode (Host), 3 - Station+AP (Dual Mode)
  delay(1500);
  esp8266.println("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"");  // Giving AP Name and Password for connection
  delay(1500);
}

void loop() 
{
  int flameSensor = flame();
  int smokeSensor = smoke();
  int airSensor = air();
  DHT.read11(temp_sensor);
  int tempSensor = DHT.temperature;
  int humSensor = DHT.humidity;
  int uvSensor = uv();
  //delay(1500)      it's better if u give a delay between each esp8266 command           
  String data = "GET /update?api_key="+ API+"&"+ field1 +"="+String(airSensor)+"&"+ field2 +"="+String(flameSensor)+"&"+field3+"="+String(smokeSensor)+"&"+field4+"="+String(tempSensor)+"&"+field5+"="+String(humSensor)+"&"+field7+"="+String(uvSensor); // Url to send data to ThingSpeak Channel
  esp8266.println("AT+CIPMUX=1");
  delay(1500);
  esp8266.println("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT);  // Establishing TCP connection with Host and Port (here ThingSpeak API on port 80)
  delay(1500);
  esp8266.println("AT+CIPSEND=0," +String(data.length()+4));    // Setting up the length of the data.
  delay(1500);
  esp8266.println(data);      // Sending Data to the Url
  delay(1500);
  esp8266.println("AT+CIPCLOSE=0");   // Closing the TCP Connection

  Serial.println("Air Quality = ");
  Serial.print(airSensor);
  Serial.println("*PPM");
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(DHT.temperature); 
  Serial.println("C  ");

  //fire_smoke_alarm system
  if ((flame_detected < flameThres) || (smoke_detected > smokeThres))
  {
    Serial.print("Pin A4 flame: ");
    Serial.println(flame_detected);
    Serial.print("Pin A5 smoke: ");
    Serial.println(smoke_detected);
    Serial.println("Fire detected...! take action immediately.");
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    Serial.print("Pin A4 flame: ");
    Serial.println(flame_detected);
    Serial.print("Pin A5 smoke: ");
    Serial.println(smoke_detected);
    Serial.println("No flame detected. stay cool");
    digitalWrite(buzzer, LOW);
  }
 
  Serial.print("Distance : ");
  Serial.println(uvSensor);
  delay(1000);
  
  //container check system
  if(uvSensor > 4)
  {
    Serial.println("Container is empty, refill it");
  }
}
