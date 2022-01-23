/*
33B808F7  0000
33B88877  1
33B848B7   2
33B8C837  3
33B828D7  4
33B8A05F  up
33B810EF  rig
33B8609F  dow
33B8E01F  l
33B820DF  mode
-------------------
Гамма
0x12ED00FF  1
0x12ED807F  2
0x12ED40BF  3
0x12EDC03F  4
0x12ED20DF  5
0x12EDDA25  +
0x12ED7A85  -
0x12ED9A65   lamp
0x12EDCA35   up
0x12EDFA05   down
0x12ED5AA5   left
0x12ED3AC5   right
0x12ED0AF5   mod -
0x12ED8A75   mod +
0x12ED4AB5   mod
0x12ED1AE5   program reset
*/
//12////////////////////////////////////////////////////////////////////////////////////
#include <iarduino_IR_RX.h>
iarduino_IR_RX IR(2);

#include <FastLED.h>


#include <DS3231.h>
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
  rtc.begin();
  //rtc.setTime(1, 0, 0);     // Set the time to 12:00:00 (24hr format)

  IR.begin();
  
  LEDS.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
  LEDS.setBrightness(MAX_LUX);//яркость сигментов по умолчанию
  FastLED.show(); // Display leds array
  clearDisplay();  
  // put your setup code here, to run once:
  //rtc.setTime(23, 22, 0);     // Set the time to 12:00:00 (24hr format)
}
///////////////////////////////////////////////////////////////////////////////////////

void IRCheck()
{

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

  //Serial.print(t.hour);
  //TimeToArray();
  if(IR.check(true)){                            // Если в буфере имеются данные, принятые с пульта (удерживается кнопка)
    Serial.println(IR.data, HEX);                // Выводим код нажатой кнопки
    Serial.println(IR.length   );                // Выводим количество бит в коде
  }
  
  // put your main code here, to run repeatedly:

}
