#include "stdinclude.hpp"
#include <FastLED.h>

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

namespace component
{
    namespace ongeki_hardware
    {
#define Sensor A4
        const int LED_PIN = 12;
        CRGB lightColors[6];
        void start()
        {
            // setup pin modes for button
            pinMode(leftA, INPUT_PULLUP);
            pinMode(leftB, INPUT_PULLUP);
            pinMode(leftC, INPUT_PULLUP);
            pinMode(sideL, INPUT_PULLUP);
            pinMode(funcL, INPUT_PULLUP);
            pinMode(rightA, INPUT_PULLUP);
            pinMode(rightB, INPUT_PULLUP);
            pinMode(rightC, INPUT_PULLUP);
            pinMode(sideR, INPUT_PULLUP);
            pinMode(funcR, INPUT_PULLUP);
            pinMode(start_button, INPUT_PULLUP);

            // setup led_t
            FastLED.addLeds<WS2812B, LED_PIN, GRB>(lightColors, 6);
        }

        void read_io(raw_hid::output_data_t *data)
        {
            data->buttons[0] = digitalRead(leftA) == LOW;
            data->buttons[1] = digitalRead(leftB) == LOW;
            data->buttons[2] = digitalRead(leftC) == LOW;
            data->buttons[3] = digitalRead(sideL) == LOW;
            data->buttons[4] = digitalRead(funcL) == LOW;
            data->buttons[5] = digitalRead(rightA) == LOW;
            data->buttons[6] = digitalRead(rightB) == LOW;
            data->buttons[7] = digitalRead(rightC) == LOW;
            data->buttons[8] = digitalRead(sideR) == LOW;
            data->buttons[9] = digitalRead(funcR) == LOW;

            data->lever = analogRead(Sensor);

            if (data->buttons[4] && data->buttons[9])
            {
                data->scan = true;
            }
            else
            {
                memset(&data->aimi_id, 0, 10);
                data->scan = false;
            }
        }

        void set_led(raw_hid::led_t &data)
        {
            FastLED.setBrightness(data.ledBrightness);

            for (int i = 0; i < 3; i++)
            {
                memcpy(&lightColors[i], &data.ledColors[i], 3);
                memcpy(&lightColors[i + 3], &data.ledColors[i + 5], 3);
            }

            FastLED.show();
        }
    }
}
