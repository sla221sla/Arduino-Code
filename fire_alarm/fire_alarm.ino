int flame_sensor = A4 ;
int buzzer = 8;
int smoke_sensor = A5;
int sensorThres = 250;
int flame_detected = 0;

void setup() 
{
  
  Serial.begin(9600) ;
  pinMode(flame_sensor, INPUT) ;
  pinMode(buzzer, OUTPUT);
  pinMode(smoke_sensor, INPUT);

}

void loop() 
{
  
  flame_detected = analogRead(flame_sensor) ;
  int smoke_detected = analogRead(smoke_sensor);
  Serial.print("Pin D4 flame: ");
  Serial.println(flame_detected);
  Serial.print("Pin A5 smoke: ");
  Serial.println(smoke_detected);
  if ((flame_detected < 500) || (smoke_detected > sensorThres))
  {
    Serial.println("Fire detected...! take action immediately.");
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    Serial.println("No flame detected. stay cool");
    digitalWrite(buzzer, LOW);
  }
  delay(1000);
  
}
