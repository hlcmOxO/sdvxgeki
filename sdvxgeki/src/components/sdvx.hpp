#include "./src/Encoder/Encoder.h"
#include "stdint.h"
#include "HID-Project.h"
#include "FastLED.h"
#include "string.h"

namespace component{
    namespace sdvx{
        void buttons();
        void fx();
        void updateMousePositionLeft();
        void encFuncLeft();
        void updateMousePositionRight();
        void encFuncRight();
        void moveLed();
        void start();
        void update();
    }
}