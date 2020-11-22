#include <LiquidCrystal_1602_RUS.h>
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define DHTPIN 11
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
LiquidCrystal_1602_RUS lcd(2,3,4,5,6,7);
int serialUpdate = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  dht.begin();
  
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    Serial.println();
    while(1);
  }
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  long pressure = bmp.readPressure();
  long pressureTorr = pressure  / 133.322368421;

  lcdPrint(humidity, temperature, heatIndex, pressureTorr, pressure);

  if (serialUpdate == 10) {
    serialPrint(humidity, temperature, heatIndex, pressureTorr, pressure);
    serialUpdate = 0;
  }

  serialUpdate++;
  delay(1000);
}

void serialPrint(float humidity, float temperature, float heatIndex, long pressureTorr, long pressure) {
  Serial.println();
  Serial.print("Влажность: " + String(humidity) + "%\t\t");
  Serial.print("Температура: " + String(temperature) + " C\t");
  Serial.print("Ощущается: " + String(heatIndex) + " C\t");
  Serial.println();

  Serial.print("Давление: " + String(pressureTorr) + " мм рт. ст.\t");
  Serial.print(String(pressure) + " Па\t\t");
  Serial.println();
}

void lcdPrint(float humidity, float temperature, float heatIndex, long pressureTorr, long pressure) {
  lcd.setCursor(0, 0);
  lcd.print(temperature);
  lcd.print((char)223);
  lcd.print("C  ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print(pressureTorr);
  lcd.print(" мм.рт.ст.  ");
}
