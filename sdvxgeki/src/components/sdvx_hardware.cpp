#include "stdinclude.hpp"
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
/* WS2812B */
#define LED1_PIN 10
#define NUM_LEDS1 20
#define LED2_PIN 11
#define NUM_LEDS2 20
#define LED_PIN 12
#define NUM_LEDS 11

/*Pot*/
int Sensor=A4;//将A0引脚取名为Sensor/ A0 pin Named Sensor
int SensorRead=0;
int newdata=0;
int inputValue=0;
int a;

const uint8_t PIN_MAP[9] = {
    // fxl LA A B start C D RC fxr
    PIN_A2, 2, 5, 6, 13, 7, 8, 9, PIN_A3};

const uint8_t PIN_KEY[9] = {
    'w', 'a', 's', 'd', 't', 'j', 'k', 'l', 'o'};

Encoder encLeft(ENC_1_PIN_A, ENC_1_PIN_B);
Encoder encRight(ENC_2_PIN_A, ENC_2_PIN_B);
CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds[NUM_LEDS];

/* LED_timer */
unsigned long time;
unsigned long pre_time;
int color = 0;
void startup()
{
    // setup pin modes for button
    pinMode(leftA, INPUT_PULLUP);
    pinMode(leftB, INPUT_PULLUP);
    pinMode(leftC, INPUT_PULLUP);
    pinMode(sideL, INPUT_PULLDOWN);
    pinMode(funcL, INPUT_PULLUP);
    pinMode(rightA, INPUT_PULLUP);
    pinMode(rightB, INPUT_PULLUP);
    pinMode(rightC, INPUT_PULLUP);
    pinMode(sideR, INPUT_PULLDOWN);
    pinMode(funcR, INPUT_PULLUP);
    pinMode(start_button, INPUT_PULLUP);

    // initialize the buttons' LED:
    FastLED.addLeds<WS2812, LED1_PIN, GRB>(leds1, NUM_LEDS1); //装饰灯条
    FastLED.addLeds<WS2812, LED2_PIN, GRB>(leds2, NUM_LEDS2); //装饰灯条
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(64);
    fill_rainbow(leds1, NUM_LEDS1, 0, 12);
    fill_rainbow(leds2, NUM_LEDS2, 0, 12);
    time = millis();
    pre_time = time;

    // initialize mouse control:
    Mouse.begin();
    // initialize keyboard control:
    NKROKeyboard.begin();
}

void buttons()
{ //主按键/main keys
    for (int i = 0; i < 9; i++)
    {
        if (digitalRead(PIN_MAP[i]) == LOW)
        {
            NKROKeyboard.press(PIN_KEY[i]);
            leds[i+1] = CRGB::White;
        }
        else{
            NKROKeyboard.release(PIN_KEY[i]);
            leds[i+1] = CRGB::Black;
        }
    }

    if (digitalRead(A0) == HIGH)
        {
            NKROKeyboard.press('q');
            leds[0] = CRGB::White;
        }
        else{
            NKROKeyboard.release('q');
            leds[0] = CRGB::Black;
        }

    if (digitalRead(A1) == HIGH)
        {
            NKROKeyboard.press('p');
            leds[10] = CRGB::White;
        }
        else{
            NKROKeyboard.release('p');
            leds[10] = CRGB::Black;
        }
    FastLED.show();
    /* sdvx's encoder */
    //旋钮更新代码
}

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

void moveLed()
{ //循环移动彩虹灯条/looply move rainbow ws2812b
    time = millis();
    if (time >= pre_time + 50)
    {
        pre_time = time;
        color = (color + 10) % 256;
        fill_rainbow(leds1, NUM_LEDS1, color, 10);
        fill_rainbow(leds2, NUM_LEDS2, color, 10);
        FastLED.show();
    }
}

void lever() 
{
    SensorRead=analogRead(Sensor);//读出Sensor的值并将它赋给SensorRead/Read Sensor and wirte for SensorRead
    newdata=map(SensorRead,0,1023,0,255);
    if (newdata >= 255)
    {
      newdata=(newdata - 128);
    }
    else if (newdata <= 255)
     {
      newdata=(newdata - 127);
     }
}
void mov()
{
  static int prev_value = 0;
  a = newdata;
  Mouse.move((prev_value - a), 0, 0);
  prev_value = a;
}
