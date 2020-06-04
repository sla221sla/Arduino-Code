#include <SoftwareSerial.h>

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

String API = "N9CPD6BNJ44OVCEV";   // ThingSpeak API Key
String HOST = "api.thingspeak.com";
String PORT = "80";
String field7 = "field7";

const int trigPin = 6;
const int echoPin = 7;

long duration;
double distance;

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
  int uvSensor = uv();
  //delay(1500)      it's better if u give a delay between each esp8266 command           
  String data = "GET /update?api_key="+ API+"&"+field7+"="+String(uvSensor); // Url to send data to ThingSpeak Channel
  esp8266.println("AT+CIPMUX=1");
  delay(1500);
  esp8266.println("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT);  // Establishing TCP connection with Host and Port (here ThingSpeak API on port 80)
  delay(1500);
  esp8266.println("AT+CIPSEND=0," +String(data.length()+4));    // Setting up the length of the data.
  delay(1500);
  esp8266.println(data);      // Sending Data to the Url
  delay(1500);
  esp8266.println("AT+CIPCLOSE=0");   // Closing the TCP Connection
 
  Serial.print("Distance : ");
  Serial.println(uvSensor);
  delay(1000);
  
  //container check system
  if(uvSensor > 4)
  {
    Serial.println("Container is empty, refill it");
  }
}
