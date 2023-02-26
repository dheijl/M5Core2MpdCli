#include <Arduino.h>
#include <M5Unified.h>

#include "config.h"

#include "battery.h"
#include "buttons.h"
#include "debug.h"
#include "menu.h"
#include "mpd_commands.h"
#include "mpdcli.h"
#include "tftfunctions.h"
#include "wifi.h"

static unsigned long wifi_started = 0;

void setup()
{
    auto cfg = M5.config();
    M5.begin();

    // init debug code
    init_debug();
    // initialize buttons
    init_buttons();
    // Initialize TFT
    init_tft();
    // initialize battery
    init_battery();
    // try to load configuration from flash or SD
    while (!load_config()) {
        tft_clear();
        tft_println_error("Missing  config!!");
        tft_println_highlight("Insert SD with config");
        vTaskDelay(3000);
    }
    // start wifi
    if (start_wifi()) {
        tft_clear();
        show_mpd_status();
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
        show_mpd_status();
        wifi_started = millis();
    }
    if (M5.BtnB.wasPressed()) {
        start_wifi();
        show_menu();
        wifi_started = millis();
    }
    if (M5.BtnC.wasPressed()) {
        start_wifi();
        toggle_mpd_status();
        wifi_started = millis();
    }
    tft_sleep();
    if ((is_wifi_connected() && (millis() - wifi_started) > wifi_timer)) {
        stop_wifi();
        if (!M5.Power.isCharging()) {
            M5.Power.powerOff();
        }
    }
    vTaskDelay(1);
}
