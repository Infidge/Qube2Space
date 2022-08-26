#include <Adafruit_CCS811.h>
Adafruit_CCS811 ccs;

#include <LoRa.h>
#include <SoftwareSerial.h>

#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial gpsSerial(3,4);

#include <SPI.h>
#include <SD.h>

#include <SHT21.h>
SHT21 sht;

#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;

File myFile;

float Temp=0.0;          
float Humidity=0.0;
float maxAlt=0.0;
float seaLevelPressure = 1009.0;
float lat;
float lon;

bool GPS = false;

int LEDpin = 3;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Wire.begin();
  pinMode(LEDpin,OUTPUT);
  digitalWrite(LEDpin,LOW);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,    
                  Adafruit_BMP280::SAMPLING_X2,  
                  Adafruit_BMP280::SAMPLING_X16, 
                  Adafruit_BMP280::FILTER_X16,  
                  Adafruit_BMP280::STANDBY_MS_500);

  LoRa.begin(433E6);
  LoRa.setTxPower(20);
  digitalWrite(LEDpin,HIGH);
}


void loop() { 
  Temp = sht.getTemperature();
  Humidity = sht.getHumidity();
  
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  myFile = SD.open("temp.txt", FILE_WRITE);
  myFile.print("Temperature = ");
  myFile.println(bmp.readTemperature());
  myFile.println(" *C");
  myFile.close();
  
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure()/100); 
  Serial.println(" hPa");
  myFile = SD.open("pressure.txt", FILE_WRITE);
  myFile.print("Pressure = ");
  myFile.println(bmp.readPressure()/100);
  myFile.println(" hPa");
  myFile.close();
  
  Serial.print("Approx altitude = ");
  Serial.print(bmp.readAltitude(seaLevelPressure)); 
  Serial.println(" m");
  myFile = SD.open("alt.txt", FILE_WRITE);
  myFile.print("Altitude = ");
  myFile.println(bmp.readAltitude(seaLevelPressure));
  myFile.println(" m");
  myFile.close();
  if (bmp.readAltitude(seaLevelPressure) > maxAlt)
    maxAlt = bmp.readAltitude(seaLevelPressure);                   

  Serial.print("Humidity = ");
  Serial.print(Humidity);
  Serial.println(" %");
  myFile = SD.open("hum.txt", FILE_WRITE);
  myFile.print("Humidity = ");
  myFile.println(Humidity);
  myFile.println(" %");
  myFile.close(); 

  Serial.print("CO2 = ");
  Serial.print(ccs.geteCO2());
  Serial.println(" ppm");
  myFile = SD.open("co2.txt", FILE_WRITE);
  myFile.print("CO2 = ");
  myFile.println(ccs.geteCO2());
  myFile.println(" ppm");
  myFile.close(); 

  Serial.print("VOC = ");
  Serial.print(ccs.getTVOC());
  Serial.println(" ppm");
  myFile = SD.open("voc.txt", FILE_WRITE);
  myFile.print("VOC = ");
  myFile.println(ccs.getTVOC());
  myFile.println(" ppm");
  myFile.close();  

  if (maxAlt > 3000 && 1000 > bmp.readAltitude(seaLevelPressure) && !GPS)
   GPS = true;
   
  if (GPS){
    if(gps.encode(gpsSerial.read()))
      gps.f_get_position(&lat,&lon); 
    LoRa.beginPacket();
    LoRa.print(lat);
    LoRa.print(" ");
    LoRa.print(lon);
    LoRa.endPacket();
  }
    
  Serial.println();
  delay(500);
}
