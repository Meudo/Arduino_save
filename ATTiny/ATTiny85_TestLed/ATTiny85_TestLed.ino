int ledPin=3;


void setup()
{
   pinMode(ledPin, OUTPUT); 
   digitalWrite(ledPin, HIGH); 
}

void loop()
{
  delay(10);
  digitalWrite(ledPin, LOW); 
  delay(90);
  digitalWrite(ledPin, HIGH);
}
