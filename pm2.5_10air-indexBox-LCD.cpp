#include <Arduino.h>
#include <Wire.h>

 //#define LCD_I2C
 //#define OLED_I2C

#ifdef LCD_I2C
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#endif

#ifdef OLED_I2C
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ 8);
#endif

int i;
int a[10] = {};
boolean stringComplete = false;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(100);
  ///////////////////////////////////
#ifdef OLED_I2C
  u8g2.begin();
#endif

  //////////////////////////////////
#ifdef LCD_I2C
  lcd.init();
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("Boot...");
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++)
  {
    lcd.write(0xff);
    delay(80);
  }
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.print("Air PM2.5 index");
  lcd.setCursor(10, 1);
  lcd.print("ug/m^3");

  //  lcd.setCursor(0, 1);
  //  lcd.print("PM10.0=");
  //  lcd.setCursor(7, 1);
  //  lcd.print("ug/m^3");
#endif
  ///////////////////////////////////
}

void loop() {
  while (Serial1.available() >= 10)
  {
    if (Serial1.read() == 0xAA)
    { a[0] = 0xAA;
      for (i = 1; i <= 9; i++)
      {
        a[i] = Serial1.read();
      }
      ///////////
      float pm25val = (a[3] * 256 + a[2]) / 10.0;
      float pm10val = (a[5] * 256 + a[4]) / 10.0;
      /////////////sourcevalue
      for (int c = 0; c <= 9; c++)
      {
        Serial.print(a[c], HEX);
        Serial.print(' ');
      }
      ///////////////pm2.5
      Serial.print(" PM2.5 =");
      Serial.print(pm25val);
      Serial.print("ug/m3");
      ///////////////pm10
      Serial.print(" PM10 =");
      Serial.print(pm10val);
      Serial.print("ug/m3");
      Serial.println("");
      //Serial.print(a[0],HEX);
      //Serial.println(a[1],HEX);
      
      ///////////////lcePrint
#ifdef LCD_I2C
      lcd.setCursor(0, 1);
      lcd.print(pm25val);
#endif
      ///////////////OLEDprint
#ifdef OLED_I2C
      u8g2.clearBuffer();          // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB14_tr); // choose a suitable font
      u8g2.drawStr(0, 20, "pm25val"); // write something to the internal memory
      u8g2.sendBuffer();          // transfer internal memory to the display
      delay(1000);
#endif
    }
  }

}
