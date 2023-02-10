#pragma once

#include "config.h"

void init_debug()
{
#ifdef DEBUG
    Serial.begin(115200);
    int n = 250;
    while (!Serial && n-- > 0) {
        delay(1);
    }
    delay(100);
#endif
}
