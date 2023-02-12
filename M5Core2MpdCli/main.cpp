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
    // check for SD card with configuration
    if (load_SD_config()) {
        if (save_FLASH_config()) {
            tft_println("Config saved to FLASH");
        }
    } else {
        if (!load_FLASH_config()) {
            tft_println("NO FLASH CONFIG!");
            while (true) { }
        }
    }
    // start wifi
    if (start_wifi(get_config())) {
        tft_clear();
        show_mpd_status();
        wifi_started = millis();
    } else {
        tft_println("Connection FAIL!");
        delay(1000);
    }
    tft_clear();
}

void loop()
{
    M5.update();
    unsigned long wifi_timer = 5000UL;
    if (M5.BtnA.wasPressed()) {
        start_wifi(get_config());
        wifi_started = millis();
        show_mpd_status();
        wifi_started = millis();
    }
    if (M5.BtnB.wasPressed()) {
        start_wifi(get_config());
        show_menu();
        wifi_started = millis();
    }
    if (M5.BtnC.wasPressed()) {
        start_wifi(get_config());
        toggle_mpd_status();
        wifi_started = millis();
    }
    if ((is_wifi_connected() && (millis() - wifi_started) > wifi_timer)) {
        stop_wifi();
        M5.Power.powerOff();
    }
    delay(1);
}
