#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#include "Seeed_BME280.h"

#define buz PB9
#define FIRE_A PA3
#define FIRE_M PA6

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
BME280 bme280;

float z, alt;

void setup() {
   pinMode(buz,OUTPUT);
   pinMode(FIRE_A,OUTPUT);
   pinMode(FIRE_M,OUTPUT);
   Serial.begin(9600);

  if (!bme280.init()) 
  {
      Serial.println("Ooops, no BME280 detected ... Check your wiring!");
      for (byte i = 0; i < 3; i++)
      {
        digitalWrite(buz, HIGH);
        delay(1000);
        digitalWrite(buz, LOW);
        delay(1000);
      }
    while(1)
    {
      Serial.println("BME ERROR...");
    }
  }
    
  if(!accel.begin())
  {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    for (byte i = 0; i < 4; i++)
      {
        digitalWrite(buz, HIGH);
        delay(1000);
        digitalWrite(buz, LOW);
        delay(1000);
      }
    while(1)
    {
      Serial.println("ADXL ERROR...");
    }
  }

  for (byte i = 0; i < 2; i++)
      {
        digitalWrite(buz, HIGH);
        delay(1000);
        digitalWrite(buz, LOW);
        delay(1000);
      }
  Serial.println("SISTEM BASLATILIYOR");
}

void loop() {
  alt = bme280.calcAltitude(bme280.getPressure());
  delay(20);
  z = acc();
  delay(20);

  Serial.print(alt);Serial.print("\t");Serial.println(z);
  
  if (z < 0)
  {
    z = acc();
    delay(20);
    if (z < 0)
    {
       z = acc();
       delay(20);
       if (z < 0)
       {
           z = acc();
           delay(20);
           if (z < 0)
           {
               z = acc();
               delay(20);
               if (z < 0)
               {
                  digitalWrite(FIRE_A, HIGH);
                  digitalWrite(FIRE_M, HIGH);
                  delay(3000);
                  digitalWrite(FIRE_A, LOW);
                  digitalWrite(FIRE_M, LOW);
                  delay(20);
               }
           }
        }
    }
  }

}

float acc ()
{
  sensors_event_t event; 
  accel.getEvent(&event);

  return event.acceleration.z;
}
