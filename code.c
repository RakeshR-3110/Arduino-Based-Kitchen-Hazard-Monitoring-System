#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 4
#define FLAME_SENSOR_PIN 3
#define BUZZER_PIN 5
#define RELAY_PIN 6
#define GAS_SENSOR_PIN A0

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin();
  lcd.backlight();
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  lcd.setCursor(0, 0);
  lcd.print(" Kitchen Safety ");
  lcd.setCursor(0, 1);
  lcd.print("   System Init   ");
  delay(2000);
  lcd.clear();
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int gasLevel = analogRead(GAS_SENSOR_PIN);
  int flameDetected = digitalRead(FLAME_SENSOR_PIN);
  bool danger = false;

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print((char)223);
  lcd.print("C H:");
  lcd.print(hum);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Gas:");
  lcd.print(gasLevel);
  lcd.print(" ");

  if (gasLevel > 400) {
    lcd.setCursor(10, 1);
    lcd.print("GAS!");
    danger = true;
  }

  if (flameDetected == LOW) {
    lcd.setCursor(10, 1);
    lcd.print("FIRE");
    danger = true;
  }

  if (danger) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
  }

  delay(2000);
}
