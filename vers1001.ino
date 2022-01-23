#include <FastLED.h>
#include "button.h" // файл лежит в одной папке со скетчем
bool flag = false;
uint32_t btnTimer = 0;

#include <DS3231.h>


// приёмник на D2
#define IR_1    0xA2
#define IR_2    0x62
#define IR_3    0xE2
#define IR_4    0x22
#define IR_5    0x2
#define IR_6    0xC2
#define IR_7    0xE0
#define IR_8    0xA8
#define IR_9    0x90
#define IR_0    0x98
#define IR_UP   0x18
#define IR_LEFT 0x10
#define IR_OK   0x38
#define IR_RIGHT 0x5A
#define IR_DOWN 0x4A
#include <NecDecoder.h>
NecDecoder ir;
byte status = 0;       // текущий статус ic
DS3231  rtc(SDA, SCL);
Time  t;

#define NUM_LEDS 58
#define COLOR_ORDER GRB  // Определяем порядок цветов для ленты
//#define COLOR_ORDER BRG  // Определяем порядок цветов для ленты
#define DATA_PIN 6  // ПИН
CRGB leds[NUM_LEDS];
///////////////////////////////////////////////////////////////////////////////////////
byte digits[16][14] = {{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//0           000
                       {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},//1          0   0
                       {1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0},//2          0   0
                       {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},//3          0   0
                       {1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1},//4           000
                       {1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},//5          0   0
                       {1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//6          0   0
                       {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1},//7          0   0
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//8           000
                       {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},//9
             {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},//C
             {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},//0*
             {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},//Г
             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//А
             {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//Р
             {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}//Э
             };
//////////////////////////////////////////////////////////////////////////////////////
CRGB ledColor = CRGB::Black; // Color used (in hex)
#define MAX_LUX 30

///////////
bool Dot = true;  //переменная дл GetTime  и  TimeToArray
uint8_t current_mode = 0; // текущий режим кнопки mode
uint8_t last_digit = 0; // переменная для TimeToArray для вкл рандом и fadefonction
//////////

void setup() {
  Serial.begin(9600);
  attachInterrupt(0, irIsr, FALLING);
  rtc.begin();
  //rtc.setTime(1, 0, 0);     // Set the time to 12:00:00 (24hr format)
  LEDS.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
  LEDS.setBrightness(MAX_LUX);//яркость сигментов по умолчанию
  FastLED.show(); // Display leds array
  clearDisplay();  
  // put your setup code here, to run once:
  //rtc.setTime(23, 22, 0);     // Set the time to 12:00:00 (24hr format)
}
///////////////////////////////////////////////////////////////////////////////////////
void irIsr() {
  ir.tick();
}
void IRCheck()
{
    if (ir.available()) {
    switch (ir.readCommand()) {
      // выводим в порт, тут может быть ваш код
      case IR_1: Serial.println("Pressed 1"); break;
      case IR_2: Serial.println("Pressed 2"); break;
      case IR_3: Serial.println("Pressed 3"); break;
      case IR_4: Serial.println("Pressed 4"); break;
      case IR_5: Serial.println("Pressed 5"); break;
      case IR_6: Serial.println("Pressed 6"); break;
      case IR_7: Serial.println("Pressed 7"); break;
      case IR_8: Serial.println("Pressed 8"); break;
      case IR_9: Serial.println("Pressed 9"); break;
    }
  }
  }
//////////////////////////////////////////////////////////////////////////////////////
int GetTime()
{
  t = rtc.getTime();
  int hour = t.hour;
  int minutes = t.min;
  int second = t.sec;
  if (second % 2 == 0) {
    Dot = false;
  }
  else {
    Dot = true;
  };
  return (hour * 100 + minutes);  
  }
//////////////////////////////////////////////////////////////////////////////////////
void TimeToArray(){
//------------------ ColorTable массив цветов------------
  long ColorTable[16] = {
    CRGB::Amethyst,
    CRGB::Aqua,
    CRGB::Blue,
    CRGB::Chartreuse,
    CRGB::DarkGreen,
    CRGB::DarkMagenta,
    CRGB::DarkOrange,
    CRGB::DeepPink,
    CRGB::Fuchsia,
    CRGB::Gold,
    CRGB::GreenYellow,
    CRGB::LightCoral,
    CRGB::Tomato,
    CRGB::Salmon,
    CRGB::Red,
      CRGB::Orchid
  };

//------------------------------------------------------
  int Now = GetTime();
  uint8_t cursor =  0; //116
  if (Dot){
    leds[28] = ledColor;  //Miganie dwukropkiem
    leds[29] = ledColor;
  }
  else{
    leds[28] = 0x000000;
    leds[29] = 0x000000;
  }
  for (uint8_t i = 1; i <= 4; i++){
    uint8_t digit = Now % 10; // получить последнюю цифру вовремя
          ////// ДЛя 4 сегмента///////////////////////////////////////////////////////
    if (i == 1){
      cursor = 44; //82
      //Serial.print(t.hour);
      //Serial.print("-");
      //Serial.print(t.min);
      //Serial.print("-");
      //Serial.println(t.sec);
      //Serial.println(Now);
      for (uint8_t k = 0; k <= 13; k++){
        if(current_mode == 2){
          leds[cursor]=0x000000;
          cursor ++;
        }
        else{
              if(current_mode == 1){//если режим Mode 1
              //if(Dot){//если Dot true
              if (digits[digit][k] == 1){
                //сигменты с 1 светят ledColor
                leds[cursor] = ledColor;
                }
                else if (digits[digit][k] == 0){
                  //сигменты с 0 Black
                  leds[cursor] = 0x000000;
                  };
                  //}
                  //else{
                  //  leds_sig[cursor] = 0x000000;//если Dot false , весь сигмент Black
                  //  }
                    }
                    else if(current_mode != 1){
                      if (digits[digit][k] == 1){
                        leds[cursor] = ledColor;
                      }
                        else if (digits[digit][k] == 0){
                          leds[cursor] = 0x000000;
                        };
                    }
                          //Serial.print(digits[digit][k]);
                          cursor ++;
        }
      }; // fin for
                          //Serial.println();
        if (digit != last_digit){
                            //fadefonction();
          ledColor = ColorTable[random(16)];
        }
      last_digit = digit;
      }// fin if
            ///////Для 3 сигмента////////////////////////////////////////////////////
      else if (i == 2) {

            cursor = 30;

            //Serial.print("Digit 3 is : ");
            //Serial.print(digit);
            //Serial.print(", the array is : ");

      for (uint8_t k = 0; k <= 13; k++){
        if(current_mode == 2){
          leds[cursor]=0x000000;
          cursor ++;
        }
        else{
        if(current_mode == 1){//если режим Mode 1
          //if(Dot){//если Dot true
          if (digits[digit][k] == 1){
            //сигменты с 1 светят ledColor
            leds[cursor] = ledColor;
          }
          else if (digits[digit][k] == 0){
            //сигменты с 0 Black
            leds[cursor] = 0x000000;
          };
                  //}
                  //else{
                  //  leds_sig[cursor] = 0x000000;//если Dot false , весь сигмент Black
                  //  }
        }
        else if(current_mode != 1){
          if (digits[digit][k] == 1){
            leds[cursor] = ledColor;
          }
          else if (digits[digit][k] == 0){
            leds[cursor] = 0x000000;
          };
        }
        //Serial.print(digits[digit][k]);
        cursor ++;
        }
      }; // fin for

        //Serial.println();

        }// fin if
//////////////////Для 2 сигмента//////////////////////////////////////////////////////
        else if (i == 3) {

            cursor = 14;

            //Serial.print("Digit 2 is : ");
            //Serial.print(digit);
            //Serial.print(", the array is : ");

        for (uint8_t k = 0; k <= 13; k++){
          if(current_mode == 1){//если режим Mode 1
            leds[cursor] = 0x000000;
          cursor ++;
          }
          else{
            if(current_mode == 2){//если режим Mode 1
              //if(Dot){//если Dot true
                if (digits[digit][k] == 1){
                  //сигменты с 1 светят ledColor
                  leds[cursor] = ledColor;
                }
                else if (digits[digit][k] == 0){
                  //сигменты с 0 Black
                  leds[cursor] = 0x000000;
                };
              //}
              //else{
            //    leds_sig[cursor] = 0x000000;//если Dot false , весь сигмент Black
              //}
            }
            else if(current_mode != 2){
              if (digits[digit][k] == 1){
                leds[cursor] = ledColor;
              }
              else if (digits[digit][k] == 0){
                leds[cursor] = 0x000000;
              };
            }
            //Serial.print(digits[digit][k]);
            cursor ++;
          }
        }; // fin for

            //Serial.println();

        }// fin if
///////////////Для 1 сигмента//////////////////////////////////////////////////////////
        else if (i == 4) {

            cursor = 0;

            //Serial.print("Digit 1 is : ");
            //Serial.println(digit);
            //Serial.print(", the array is : ");

      for (uint8_t k = 0; k <= 13; k++){
        if(current_mode == 1){//если режим Mode 1
          leds[cursor] = 0x000000;
          cursor ++;
        }
        else{
          if(current_mode == 2){//если режим Mode 1
            //if(Dot){//если Dot true
              if (digits[digit][k] == 1){
                //сигменты с 1 светят ledColor
                leds[cursor] = ledColor;
              }
              else if (digits[digit][k] == 0){
                //сигменты с 0 Black
                leds[cursor] = 0x000000;
              };
            //}
            //else{
            //  leds_sig[cursor] = 0x000000;//если Dot false , весь сигмент Black
            //}
          }
          else if(current_mode != 2){
            if (digits[digit][k] == 1){
              leds[cursor] = ledColor;
            }
            else if (digits[digit][k] == 0){
              leds[cursor] = 0x000000;
            };
          }
          //Serial.print(digits[digit][k]);
          cursor ++;
        }
      }; // fin for

            //Serial.println();

        }// fin if
        Now /= 10;
        //Serial.println(Now);
    }

}


void clearDisplay() { /// чистка дисплеея
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i].setHSV(0, 0, 0);
  }
}


void loop() {
  TimeToArray();
  //FastLED.show(); // Display leds array
  FastLED.show(); // Display leds array
  //FastLED.delay(8);
  clearDisplay();  
   if (ir.available()) {
    Serial.print(t.hour);
    switch (ir.readCommand()) {
      // выводим в порт, тут может быть ваш код
      case IR_1: Serial.println("Pressed 1"); break;
      case IR_2: Serial.println("Pressed 2"); break;
      case IR_3: Serial.println("Pressed 3"); break;
      case IR_4: Serial.println("Pressed 4"); break;
      case IR_5: Serial.println("Pressed 5"); break;
      case IR_6: Serial.println("Pressed 6"); break;
      case IR_7: Serial.println("Pressed 7"); break;
      case IR_8: Serial.println("Pressed 8"); break;
      case IR_9: Serial.println("Pressed 9"); break;
    }
   }
  //Serial.print(t.hour);
  //TimeToArray();

  
  // put your main code here, to run repeatedly:

}
