#include <TinyGPS++.h>
#include <SoftwareSerial.h>
String text = "";
static const int RXPin = D7, TXPin = D8;
static const uint32_t GPSBaud = 9600;
#define AUX D6

TinyGPSPlus gps;

SoftwareSerial LORA(RXPin, TXPin);
void setup()
{
  Serial.begin(9600);
  LORA.begin(GPSBaud);
}

void loop()
{
  text = "";
  while (Serial.available() > 0)
    if (gps.encode(Serial.read()))
      displayInfo();

}

void displayInfo()
{
  
  if (gps.location.isValid())
  {
    text = "LAT:" + String(gps.location.lat(),6) + "LNG:" + String(gps.location.lng(),6);

  }
  else
  {
    text = "INVALID";
  }

  if (gps.time.isValid())
  {
    text = text + "SEC:" + String(gps.time.second());
  }

  if (digitalRead(AUX) == 1)
  {
  LORA.println(text);
  LORA.flush();
  }
}
