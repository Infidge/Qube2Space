#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
void setup() {
  Serial.begin(9600);
  LoRa.begin(433E6);
  lcd.begin(16, 2);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    lcd.clear();
    lcd.setCursor(0, 1);
    while (LoRa.available())
      lcd.println((char)LoRa.read());
  }
  delay(500);
}
