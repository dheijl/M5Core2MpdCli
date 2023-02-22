#include "config.h"

#include "sdcard_fs.h"
#include "tftfunctions.h"
#include "utils.h"

#include <SD.h>

#include <M5Unified.h>

#define TFCARD_CS_PIN GPIO_NUM_4

bool read_wifi_SD(WIFI_ACC_PT& ap)
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
        result = parse_wifi_file(wifif, ap);
    } else {
        tft_println_error("error reading wifi.txt");
        vTaskDelay(1000);
    }
    SD.end();
    return result;
}

bool read_players_SD(vector<MPD_PLAYER*>& players)
{
    bool result = false;
    if (!SD.begin(TFCARD_CS_PIN, SPI, 25000000)) {
        SD.end();
        return result;
    }
    tft_println_highlight("Loading players");
    File plf = SD.open("/players.txt", FILE_READ);
    if (plf) {
        result = parse_players_file(plf, players);
    } else {
        tft_println_error("error reading players.txt");
        vTaskDelay(1000);
    }

    SD.end();
    return result;
}

bool read_favourites_SD(vector<FAVOURITE*>& favourites)
{
    bool result = false;
    if (!SD.begin(TFCARD_CS_PIN, SPI, 25000000)) {
        SD.end();
        return result;
    }
    tft_println_highlight("Loading favourites");
    File favf = SD.open("/favs.txt", FILE_READ);
    if (favf) {
        result = parse_favs_file(favf, favourites);
    } else {
        tft_println_error("error reading favs.txt");
        vTaskDelay(1000);
    }

    SD.end();
    return result;
}

static bool parse_wifi_file(File wifif, WIFI_ACC_PT& ap)
{
    bool result = false;
    tft_println("Loading WiFi ssid/psw");
    while (wifif.available()) {
        String line = wifif.readStringUntil('\n');
        line.trim();
        string wifi = line.c_str();
        DPRINT(wifi.c_str());
        if (wifi.length() > 1) {
            vector<string> parts = split(wifi, '|');
            if (parts.size() == 2) {
                ap.ssid = strdup(parts[0].c_str());
                ap.psw = strdup(parts[1].c_str());
                result = true;
            }
        }
    }
    wifif.close();
    return result;
}

static bool parse_players_file(File plf, vector<MPD_PLAYER*>& players)
{
    bool result = false;
    tft_println("Loading players:");
    while (plf.available() && (players.size() <= 5)) { // max 5 players
        String line = plf.readStringUntil('\n');
        line.trim();
        string pl = line.c_str();
        DPRINT(pl.c_str());
        if (pl.length() > 1) {
            vector<string> parts = split(pl, '|');
            if (parts.size() == 3) {
                MPD_PLAYER* mpd = new MPD_PLAYER();
                mpd->player_name = strdup(parts[0].c_str());
                mpd->player_ip = strdup(parts[1].c_str());
                mpd->player_port = stoi(parts[2]);
                players.push_back(mpd);
                tft_println(String(mpd->player_name) + " " + String(mpd->player_ip) + ":" + String(mpd->player_port));
            }
        }
    }
    plf.close();
    if (players.size() > 0) {
        result = true;
    } else {
        tft_println("No players!");
    }
    return result;
}

static bool parse_favs_file(File favf, vector<FAVOURITE*>& favourites)
{
    bool result = false;
    tft_println("Loading favourites");
    while (favf.available() && favourites.size() <= 50) { // max 50
        String line = favf.readStringUntil('\n');
        line.trim();
        string fav = line.c_str();
        DPRINT(fav.c_str());
        if (fav.length() > 1) {
            vector<string> parts = split(fav, '|');
            if (parts.size() == 2) {
                FAVOURITE* f = new FAVOURITE();
                f->fav_name = strdup(parts[0].c_str());
                f->fav_url = strdup(parts[1].c_str());
                favourites.push_back(f);
            }
        }
    }
    favf.close();
    if (favourites.size() > 0) {
        tft_println("Loaded " + String(favourites.size()) + " favourites");
        result = true;
    } else {
        tft_println("No favourites!");
    }
    return result;
}
