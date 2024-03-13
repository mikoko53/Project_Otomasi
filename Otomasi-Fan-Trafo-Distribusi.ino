#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define ONE_WIRE_BUS D5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


DeviceAddress sensor1 = { 0x28, 0xFF, 0x64, 0x1E, 0xC, 0x31, 0x31, 0xD9 };
DeviceAddress sensor2 = { 0x28, 0xFF, 0x64, 0x1E, 0xC, 0xD, 0xBB, 0x4B };

const int Relay1 = D4;
const int Relay2 = D3;
void setup(void)
{
  Serial.begin(115200);
  sensors.begin();
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  sensors.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop(void)
{
  sensors.requestTemperatures(); // Send the command to get temperatures
  float temp1 = sensors.getTempC(sensor1) + 2.50 ; 
  float temp2 = sensors.getTempC(sensor2) + 6.00;
  //Serial.println(temp1);
  //Serial.println(temp2);
  if (temp1 < 42.00)
  {
    digitalWrite(Relay1, LOW);//relay mati
  }
  if (temp1 >= 45.00)
  {
    digitalWrite(Relay1, HIGH); //relay on
  }
  if (temp2 < 42.00)
  {
    digitalWrite(Relay2, LOW);//relay mati
  }
  if (temp2 >= 45.00)
  {
    digitalWrite(Relay2, HIGH); //relay on
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temp (630KVA): ");
  display.setTextSize(2);
  display.setCursor(0, 16);
  display.print(temp1);
  //display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  display.setTextSize(1);
  display.setCursor(0, 34);
  display.print("Temp(1250KVA): ");
  display.setTextSize(2);
  display.setCursor(0, 47);
  display.print(temp2);
  //display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  display.display();
  delay(2000);
}
