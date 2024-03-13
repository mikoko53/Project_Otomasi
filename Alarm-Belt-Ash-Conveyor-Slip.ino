unsigned long lastflash;
int RPM1;
void ICACHE_RAM_ATTR sens1() {
  RPM1++;
}

/*int RPM2;
void ICACHE_RAM_ATTR sens2() {
  RPM2++;
}
*/
void setup() {
  Serial.begin(115200);
  pinMode(D5, OUTPUT);
  pinMode(D3, INPUT_PULLUP);
  //pinMode(D5, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D3), sens1, FALLING);
  //attachInterrupt(digitalPinToInterrupt(D5), sens2, RISING);
  //SENSOR: GPIO0 (NodeMCU - D3)
}

void loop() {
  noInterrupts();
  int wings1 = 1; // no of wings of rotating object, for disc object use 1 with white tape on one side
  int RPMnew1 = RPM1 / wings1; //here we used fan which has 3 wings
  //int wings2 = 4; // no of wings of rotating object, for disc object use 1 with white tape on one side
  //nt RPMnew2 = RPM2 / wings2; //here we used fan which has 3 wings
  Serial.print(RPMnew1);
  Serial.print(" Rot/sec :");  //Revolutions per second
  Serial.print((RPMnew1 * 60));
  //Serial.println("Rot/min. ");   //Revolutions per minut
  //Serial.print(RPMnew2);
  //Serial.print(" Rot/sec :");  //Revolutions per second
  //Serial.print((RPMnew2 * 60));
  //Serial.println("Rot/min. ");   //Revolutions per minut
  //if (RPMnew1 > 0 && RPMnew2 == 0)
  //{
 //   digitalWrite(D4, HIGH);
  //}
  if (RPMnew1 == 0)
  {
    digitalWrite(D5, HIGH);
  }
  if (RPMnew1 > 0 )
  {
    digitalWrite(D5, LOW);
  }
  RPM1 = 0;
  //RPM2 = 0;
  interrupts();
  delay(7000);  //11 second.
}
