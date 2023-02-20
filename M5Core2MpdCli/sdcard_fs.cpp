#include "config.h"

#include "sdcard_fs.h"
#include "tftfunctions.h"

#include <SD.h>

#include <M5Unified.h>

#define TFCARD_CS_PIN GPIO_NUM_4

bool read_wifi_SD(CONFIG& config)
{
    bool result = false;
    if (!SD.begin(TFCARD_CS_PIN, SPI, 25000000)) {
        SD.end();
        return result;
    }
    tft_println_highlight("SD card present!");
    vTaskDelay(50);
    tft_println_highlight("Loading wifi");
    File wifif = SD.open("/wifi.txt", FILE_READ);
    if (wifif) {
        result = parse_wifi_file(wifif, config);
    } else {
        tft_println_error("error reading wifi.txt");
        vTaskDelay(1000);
    }
    SD.end();
    return result;
}

bool read_players_SD(CONFIG& config)
{
    bool result = false;
    if (!SD.begin(TFCARD_CS_PIN, SPI, 25000000)) {
        SD.end();
        return result;
    }
    tft_println_highlight("Loading players");
    File plf = SD.open("/players.txt", FILE_READ);
    if (plf) {
        result = parse_players_file(plf, config);
    } else {
        tft_println_error("error reading players.txt");
        vTaskDelay(1000);
    }

    SD.end();
    return result;
}

bool read_favourites_SD(CONFIG& config)
{
    bool result = false;
    if (!SD.begin(TFCARD_CS_PIN, SPI, 25000000)) {
        SD.end();
        return result;
    }
    tft_println_highlight("Loading favourites");
    File favf = SD.open("/favs.txt", FILE_READ);
    if (favf) {
        result = parse_favs_file(favf, config);
    } else {
        tft_println_error("error reading favs.txt");
        vTaskDelay(1000);
    }

    SD.end();
    return result;
}
