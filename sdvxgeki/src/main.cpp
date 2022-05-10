#include "stdinclude.hpp"
#include "./components/src/Encoder/Encoder.h"
#include "FastLED.h"
#include "string.h"
void setup()
{
  component::manager::start();
}

void loop()
{
  component::manager::update();
}