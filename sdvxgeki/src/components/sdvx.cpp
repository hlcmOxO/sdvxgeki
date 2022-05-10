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

namespace component
{
    namespace sdvx
    {
        const uint8_t PIN_MAP[7] = {
            // A B C D start fxl fxr
            5, 6, 7, 8, 13, PIN_A2, PIN_A3};

        const uint8_t PIN_KEY[7] = {
            's', 'd', 'j', 'k', 't', 'w', 'o'};

        Encoder encLeft(ENC_1_PIN_A, ENC_1_PIN_B);
        Encoder encRight(ENC_2_PIN_A, ENC_2_PIN_B);

        CRGB leds1[NUM_LEDS1];
        CRGB leds2[NUM_LEDS2];

        /* LED_timer */
        unsigned long time;
        unsigned long pre_time;
        int color = 0;
        void start()
        {
            // initialize the buttons' LED:
            FastLED.addLeds<WS2812, LED1_PIN, GRB>(leds1, NUM_LEDS1); //装饰灯条
            FastLED.addLeds<WS2812, LED2_PIN, GRB>(leds2, NUM_LEDS2); //装饰灯条
            FastLED.setBrightness(128);
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
            for (int i = 0; i < 7; i++)
            {
                if (digitalRead(PIN_MAP[i]) == LOW)
                {
                    NKROKeyboard.press(PIN_KEY[i]);
                }
            }
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
            if (time >= pre_time + 250)
            {
                pre_time = time;
                color = (color + 1) % 256;
                fill_rainbow(leds1, NUM_LEDS1, color, 2);
                fill_rainbow(leds2, NUM_LEDS2, color, 2);
                FastLED.show();
            }
        }

        void update()
        {
            buttons();
            encFuncLeft();
            encFuncRight();
            moveLed();
        }
    }
}