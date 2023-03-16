// Copyright (c) 2023 @dheijl (danny.heijl@telenet.be)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <Arduino.h>
#include <M5Unified.h>

#include "config.h"

#include "debug.h"
#include "menu.h"
#include "mpdcli.h"
#include "tftfunctions.h"
#include "wifi.h"

static unsigned long wifi_started = 0;
static Menu menu;

void setup()
{
    auto cfg = M5.config();
    M5.begin();

    // init debug code
    init_debug();
    // Initialize TFT
    init_tft();
    // try to load configuration from flash or SD
    while (!Config.load_config()) {
        tft_clear();
        tft_println_error("Missing  config!!");
        tft_println_highlight("Insert SD with config");
        vTaskDelay(3000);
    }
    // Create menus based on config
    menu.CreateMenus();
    // start wifi and show current MPD status of actiev player
    if (start_wifi()) {
        tft_clear();
        mpd.show_mpd_status();
        wifi_started = millis();
    } else {
        tft_println("Connection FAIL!");
        vTaskDelay(1000);
    }
    tft_clear();
}

const unsigned long wifi_timer = 10000UL;

void loop()
{
    M5.update();
    if (M5.BtnA.wasPressed()) {
        start_wifi();
        mpd.show_mpd_status();
        wifi_started = millis();
    }
    if (M5.BtnB.wasPressed()) {
        start_wifi();
        menu.Show();
        wifi_started = millis();
    }
    if (M5.BtnC.wasPressed()) {
        start_wifi();
        mpd.toggle_mpd_status();
        wifi_started = millis();
    }
    tft_sleep();
    if ((is_wifi_connected() && (millis() - wifi_started) > wifi_timer)) {
        stop_wifi();
        if (!M5.Power.isCharging()) {
            M5.Power.powerOff();
        }
    }
    vTaskDelay(5);
}
