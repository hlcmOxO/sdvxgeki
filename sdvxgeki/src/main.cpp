#include "stdinclude.hpp"
#include "./src/Encoder/Encoder.h"
#include "FastLED.h"
#include "string.h"

/*Buttons using digital pins*/
#define leftA 2 //实际上是4口
#define leftB 5
#define leftC 6
#define rightA 7
#define rightB 8
#define rightC 9
#define start_button 13

/* Buttons using analog pins */
#define sideL A0
#define sideR A1
#define funcL A2
#define funcR A3

/* Encoder */
#define ENC_1_PIN_A 0
#define ENC_1_PIN_B 1
#define ENC_2_PIN_A 4 //实际上是2口
#define ENC_2_PIN_B 3

Encoder encLeft(ENC_1_PIN_A, ENC_1_PIN_B);
Encoder encRight(ENC_2_PIN_A, ENC_2_PIN_B);

/* WS2812B */
#define LED1_PIN 10
#define NUM_LEDS1 20
#define LED2_PIN 11
#define NUM_LEDS2 20
CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];

/* LED_timer */
unsigned long time;
unsigned long pre_time;
int color = 0;

void setup()
{
  // initialize the buttons' LED:
  FastLED.addLeds<WS2812, LED1_PIN, GRB>(leds1, NUM_LEDS1); //装饰灯条
  FastLED.addLeds<WS2812, LED2_PIN, GRB>(leds2, NUM_LEDS2); //装饰灯条
  FastLED.setBrightness(128);
  fill_rainbow(leds1, NUM_LEDS1, 0, 12);
  fill_rainbow(leds2, NUM_LEDS2, 0, 12);
  time = millis();
  pre_time = time;

  component::manager::start();

  Serial.begin(115200);
  // initialize mouse control:
  Mouse.begin();
  // initialize keyboard control:
  NKROKeyboard.begin();
}

void buttons()
{ //主按键/main keys

  if (digitalRead(start_button) == LOW)
  {
    NKROKeyboard.press('t');
  }
  else
  {
    NKROKeyboard.release('t');
  }

  if (digitalRead(leftB) == LOW)
  {
    NKROKeyboard.press('s');
  }
  else
  {
    NKROKeyboard.release('s');
  }

  if (digitalRead(leftC) == LOW)
  {
    NKROKeyboard.press('d');
  }
  else
  {
    NKROKeyboard.release('d');
  }

  if (digitalRead(rightA) == LOW)
  {
    NKROKeyboard.press('j');
  }
  else
  {
    NKROKeyboard.release('j');
  }

  if (digitalRead(rightB) == LOW)
  {
    NKROKeyboard.press('k');
  }
  else
  {
    NKROKeyboard.release('k');
  }
}

void fx()
{ // fx或者func按键/fx or func
  if (digitalRead(funcL) == LOW)
  {
    NKROKeyboard.press('w');
  }
  else
  {
    NKROKeyboard.release('w');
  }

  if (digitalRead(funcR) == LOW)
  {
    NKROKeyboard.press('o');
  }
  else
  {
    NKROKeyboard.release('o');
  }
}

/* sdvx's encoder */
//旋钮更新代码

void updateMousePositionLeft()
{
  Mouse.move(encLeft.read(), 0, 0);
}

void encFuncLeft()
{
  updateMousePositionLeft();
  encLeft.write(0);
}

void updateMousePositionRight()
{
  Mouse.move(0, encRight.read(), 0);
}

void encFuncRight()
{
  updateMousePositionRight();
  encRight.write(0);
}

void keys()
{ //按键/keys
  buttons();
  fx();
}

void moveLed()
{ //循环移动彩虹灯条/looply move rainbow ws2812b
  time = millis();
  if (time >= pre_time + 250)
  {
    pre_time = time;
    color = (color + 1) % 256;
    fill_rainbow(leds1, NUM_LEDS1, color, 2);
    fill_rainbow(leds2, NUM_LEDS2, color, 2);
    FastLED.show();
  }
}

void loop()
{
  component::manager::update();
  moveLed();
  encFuncLeft();
  encFuncRight();
  keys();
}