#include <SPI.h>
#include <SD.h>
#include <SHT21.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;
SHT21 sht;
float Temp;          
float Humidity;
File myFile;
void setup() {
  Serial.begin(9600);
 Serial.println(F("BMP280 test"));

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

}

void loop() { 
  Temp = sht.getTemperature();
  Humidity = sht.getHumidity();
  
  Serial.print(F("Temperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  myFile = SD.open("temp.txt", FILE_WRITE);
  myFile.print("Temperature = ");
  myFile.println(bmp.readTemperature());
  myFile.println(" *C");
  myFile.close();
  
  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure()/100); //displaying the Pressure in hPa, you can change the unit
  Serial.println(" hPa");
  myFile = SD.open("pressure.txt", FILE_WRITE);
  myFile.print("Pressure = ");
  myFile.println(bmp.readPressure()/100);
  myFile.println(" hPa");
  myFile.close();

  Serial.print(F("Approx altitude = "));
  Serial.print(bmp.readAltitude(1009)); 
  Serial.println(" m");
  myFile = SD.open("alt.txt", FILE_WRITE);
  myFile.print("Altitude = ");
  myFile.println(bmp.readAltitude(1009));
  myFile.println(" m");
  myFile.close();                   

  Serial.print(F("Humidity = "));
  Serial.print(Humidity);
  Serial.println(" %");
  myFile = SD.open("hum.txt", FILE_WRITE);
  myFile.print("Humidity = ");
  myFile.println(Humidity);
  myFile.println(" %");
  myFile.close();   
    
  Serial.println();
  delay(500);
}
