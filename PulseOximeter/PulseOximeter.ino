#include <Arduino.h>
#include "./src/ApplicationModel.h"

ApplicationModel model;

void setup()
{
    model.Initialize();
}

void loop()
{
    model.Loop();
}


