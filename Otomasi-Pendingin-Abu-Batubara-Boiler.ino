#define trigPin D6
#define echoPin D7
#define Solenoid D4
void setup()
{
  Serial.begin (115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(Solenoid, OUTPUT);
}
void loop()
{
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  noInterrupts();
  duration = pulseIn(echoPin, HIGH);
  interrupts();
  distance = (duration / 2) / 29.1;
  if (distance >= 50)
  {
    digitalWrite(Solenoid, HIGH);
  }
  if (distance <= 40)
  {
    digitalWrite(Solenoid, LOW);
  }
  Serial.print(distance);
  Serial.println("cm");
  delay(500);
}
