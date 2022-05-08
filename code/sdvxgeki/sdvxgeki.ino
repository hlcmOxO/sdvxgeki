#include "Keyboard.h"
#include "Mouse.h"
#include "./src/Encoder/Encoder.h"
#include "FastLED.h"
#include "string.h"

/*Buttons using digital pins*/
#define leftA 4
#define leftB 5
#define leftC 6
#define rightA 7
#define rightB 8
#define rightC 9
#define start 13

/* Encoder */
#define ENC_1_PIN_A 0
#define ENC_1_PIN_B 1
#define ENC_2_PIN_A 2
#define ENC_2_PIN_B 3

Encoder encLeft(ENC_1_PIN_A, ENC_1_PIN_B);
Encoder encRight(ENC_2_PIN_A, ENC_2_PIN_B);

/* WS2812B */
#define LED1_PIN 10
#define NUM_LEDS1 20
#define LED2_PIN 11
#define NUM_LEDS2 20
#define LED_PIN 12
#define NUM_LEDS 8
CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds[NUM_LEDS];

/* Buttons using analog pins */
#define sideL A0
#define sideR A1
#define funcL A2
#define funcR A3

/* lever */
#define Sensor A4
int SensorRead = 0;
int newdata = 0;
int inputValue = 0;
int a;

/* LED_timer */
unsigned long time;
unsigned long pre_time;
int color = 0;

/* define the states and properties of the buttons */
int ButtonState = 0;
char cmd;
int leds_colors[8] = {0, 1, 2, 3, 1, 2, 3, 4};

void setup() {
  // initialize the buttons' LED:
  FastLED.addLeds<WS2812, LED1_PIN, GRB>(leds1, NUM_LEDS1); //装饰灯条
  FastLED.addLeds<WS2812, LED2_PIN, GRB>(leds2, NUM_LEDS2); //装饰灯条
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS); //按键灯光
  FastLED.setBrightness(128);
  fill_rainbow(leds1, NUM_LEDS1, 0, 12);
  fill_rainbow(leds2, NUM_LEDS2, 0, 12);
  time = millis();
  pre_time = time;
  
  // initialize the buttons' inputs:
  pinMode(leftA, INPUT_PULLUP);
  pinMode(leftB, INPUT_PULLUP);
  pinMode(leftC, INPUT_PULLUP);
  pinMode(rightA, INPUT_PULLUP);
  pinMode(rightB, INPUT_PULLUP);
  pinMode(rightC, INPUT_PULLUP);
  pinMode(start, INPUT_PULLUP);

  Serial.begin(115200);
  // initialize mouse control:
  Mouse.begin();
  // initialize keyboard control:
  Keyboard.begin();
}

void Card(){
    if (digitalRead(funcL) == LOW && digitalRead(funcR) == LOW){ //回车键刷卡/hold Enter to load card
    Keyboard.press(0xB0);
   }
   else {
    Keyboard.release(0xB0);
}
}

void buttons(){ //主按键/main keys
  if (digitalRead(leftA) == LOW){
    Keyboard.press('a');
  }
  else{
    Keyboard.release('a');
  }

  if (digitalRead(leftB) == LOW){
    Keyboard.press('s');
  }
  else{
    Keyboard.release('s');
  }
  
  if (digitalRead(leftC) == LOW){
    Keyboard.press('d');
  }
  else{
    Keyboard.release('d');
  }

  if (digitalRead(rightA) == LOW){
    Keyboard.press('j');
  }
  else{
    Keyboard.release('j');
  }

  if (digitalRead(rightB) == LOW){
    Keyboard.press('k');
  }
  else{
    Keyboard.release('k');
  }

  if (digitalRead(rightC) == LOW){
    Keyboard.press('l');
  }
  else{
    Keyboard.release('l');
  }
}

void sides(){ //侧键/side keys
  if (digitalRead(sideL) == LOW){
    Keyboard.press('q');
  }
  else{
    Keyboard.release('q');
  }

  if (digitalRead(sideR) == LOW){
    Keyboard.press('p');
  }
  else{
    Keyboard.release('p');
  }
}

void fx(){ //fx或者func按键/fx or func
  if (digitalRead(funcL) == LOW){
    Keyboard.press('w');
  }
  else{
    Keyboard.release('w');
  }

  if (digitalRead(funcR) == LOW){
    Keyboard.press('o');
  }
  else{
    Keyboard.release('o');
  }
}

void lever() //电位器摇杆/lever
{
    SensorRead=analogRead(Sensor);//读出Sensor的值并将它赋给SensorRead/Read Sensor and wirte for SensorRead
    newdata=map(SensorRead, 0, 1023, 0, 255);
    if (newdata >= 255)
    {
      newdata=(newdata - 128);
      Serial.println(newdata);
    }
    else if (newdata <= 255)
    {
      newdata=(newdata - 127);
      Serial.println(newdata);
    }
}

/* sdvx's encoder */
//旋钮更新代码
void encFuncLeft(){
  updateMousePositionLeft();
  encLeft.write(0);
}

void encFuncRight(){
  updateMousePositionRight();
  encRight.write(0);
}

void updateMousePositionLeft(){
  Mouse.move(encLeft.read(), 0, 0);
}

void updateMousePositionRight(){
  Mouse.move(0, encRight.read(), 0);
}

void move() //摇杆鼠标位置更新/lever mouse update
{
  static int prev_value = 0;
  a = newdata;
  Mouse.move((prev_value - a), 0, 0);
  prev_value = a;
}

void keys(){ //按键/keys
  buttons();
  sides();
  Card();
  fx();
}

void moveLed(){ //循环移动彩虹灯条/looply move rainbow ws2812b
  time = millis();
  if (time >= pre_time + 250){
    pre_time = time;
    color = (color + 1) % 256;
    fill_rainbow(leds1, NUM_LEDS1, color, 2);
    fill_rainbow(leds2, NUM_LEDS2, color, 2);
  }
}

void updateButtonState(){ //更新按钮亮起状态/update buttons' lights state
  if (Serial.available() > 0)
  {
    cmd = Serial.read();
    ButtonState = Serial.parseInt();
  }
  for (int s = 0; s <= NUM_LEDS; s = s + 1){
    if (ButtonState % 2 == 1){
      if (leds_colors[s] == 1){
        leds[s] = CRGB::Red;
      }
      if (leds_colors[s] == 2){
        leds[s] = CRGB::Green;
      }
      if (leds_colors[s] == 3){
        leds[s] = CRGB::Blue;
      }
      if (leds_colors[s] == 0){
        leds[s] = CRGB::Chartreuse;
      }
      if (leds_colors[s] == 4){
        leds[s] = CRGB::Amethyst;
      }
    }
    else{
        leds[s] = CRGB::Black;
    }
  }
}

void loop(){
  moveLed();
  updateButtonState();
  lever();
  encFuncLeft();
  encFuncRight();
  keys();
  move();
  FastLED.show();
}
