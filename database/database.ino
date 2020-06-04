#include <SoftwareSerial.h>

#define RX 1 // Connected to Digital Pin 0 of Arduino   TX of Wifi connected to RX of Arduino
#define TX 0 // Connected to Digital Pin 1 of Arduino   RX of Wifi conneceted to TX of Arduino

String AP = "IIIT-New";       // Your Access Point Name
String PASS = "iiitk@iisertvm18";     // Your Password

/* 
 *  Here we are sending data to ThingSpeak API. 
 *  Create a free account in ThingSpeak.
 *  Then create a new channel. 
 *  Copy the API key for your new channel to below API String
 *  You can later browse the channel visualization to view the data
*/
//String API = "EZRR91A4TJQ08FME";   // ThingSpeak API Key
String HOST = "192.168.132.33";
String PORT = "80";
String field = "vals";     //let it remain as field1 field2 etc...even if the graph name is different

int airquality()
{
  pinMode(A0, INPUT);
  int sensorValue = analogRead(A0);
  return sensorValue;
}


SoftwareSerial esp8266(RX,TX); // Initializing esp8266 package with proper pins
 
void setup() 
{
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
  int valSensor = airquality();
  //delay(1500);       it's better if u give a delay between each esp8266 command           
  String data = "GET /insert.php?"+field +"="+String(valSensor); // Url to send data to ThingSpeak Channel
  esp8266.println("AT+CIPMUX=1");
  delay(1500);
  esp8266.println("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT);  // Establishing TCP connection with Host and Port (here ThingSpeak API on port 80)
  delay(1500);
  esp8266.println("AT+CIPSEND=0," +String(data.length()+4));    // Setting up the length of the data.
  delay(1500);
  esp8266.println(data);      // Sending Data to the Url
  delay(1500);
  esp8266.println("AT+CIPCLOSE=0");   // Closing the TCP Connection
}
