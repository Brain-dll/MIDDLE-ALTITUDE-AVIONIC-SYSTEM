#include<LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>

LiquidCrystal lcd (8,7,6,5,4,3);
File myFile;

#define AUX 2
#define BUT A4

int BUT_V;
String data = "" , up = "" , down = "";
bool logg = true;

unsigned long Last,Last1;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(200);
  
  lcd.begin(16,2);
  lcd.home();
  lcd.print("SD CARD");
  lcd.setCursor(0,1);
  lcd.print("INITIALIZING...");
  delay(1000);
  if (!SD.begin(10)) {
    lcd.clear();
    lcd.home();
    lcd.print("SD CARD FAILED");
    while (1);
  }
  lcd.clear();
  lcd.home();
  lcd.print("DONE...");
  delay(1000);
  
  pinMode(AUX,INPUT);
  
  SD.remove("FLIGHT.txt");

  myFile = SD.open("FLIGHT.txt", FILE_WRITE);

  if (myFile) {
    lcd.clear();
    lcd.home();
    lcd.print("FLIGHT.txt");
    lcd.setCursor(0,1);
    lcd.print("STARTED...");
    delay(1000);
  } 
  else 
  {
    lcd.print("FLIGHT FAILED...");
    lcd.setCursor(11,1);
    lcd.print("SKIP");
    Last = millis();
    while(1)
    {
      if (millis() - Last > 500)
      {
        if (BUTTON_C() == 2)
        {
          lcd.clear();
          delay(1000);
          break;
        }
        lcd.noDisplay();
        delay(100);
        lcd.display();
        Last = millis();
      }
    }
  }
}

void loop() {
  if(Serial.available())
  {
    data = Serial.readString();
    delay(200);
      if (myFile && logg) 
      {
        myFile.println(data);
      } 
      else if (myFile && logg)
      {
      lcd.clear();
      lcd.home();
      lcd.println("ERROR FLIGHT.TXT");
      delay(500);    
    }
    lcd.clear();
    lcd.home();
    lcd.print(ITEM_up(data));
    lcd.setCursor(0,1);
    lcd.print(ITEM_down(data));
    Last = millis();
  }
  else 
  {
    if ((millis() - Last) > 1000) {
      lcd.clear();
      lcd.print("Searching: ");
      Last = millis();
      lcd.noDisplay();
      delay(100);
      lcd.display();
    }
  }
  if ( BUTTON_C() == 2)
  {
    if (logg == true)
    {
      logg = false;
      myFile.close();
      lcd.clear();
      lcd.home();
      lcd.print("STOPPED LOGGING");
      delay(1000);
    }
    else
    {
      logg = true;
      myFile = SD.open("FLIGHT.txt", FILE_WRITE);
      lcd.clear();
      lcd.home();
      lcd.print("STARTED LOGGING");
      delay(1000);
    }
  }
}

String ITEM_up(String data_)
{
  if(data_.indexOf("LAT:") != -1 && data_.indexOf("LNG:") != -1)
    up = data_.substring(data_.indexOf("LAT:"), data_.indexOf("LNG:"));
  else
    up = "NOPE";
  return up;
}
String ITEM_down(String data_)
{
  if(data_.indexOf("LAT:") != -1 && data_.indexOf("LNG:") != -1)
    down = data_.substring(data_.indexOf("LNG:"), data_.length());
  else
    down = "NOPE";
  return down;
}
int BUTTON_C()
{
  BUT_V = analogRead(BUT);
  if (BUT_V > 900)
      return 2;
  else if (450 < BUT_V < 650)
      return 1;
  else
      return -1;
}
