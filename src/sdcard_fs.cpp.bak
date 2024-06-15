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

#include "config.h"

#include "sdcard_fs.h"
#include "tftfunctions.h"
#include "utils.h"

#include <SD.h>

#include <M5Unified.h>

#include <string.h>

#define TFCARD_CS_PIN GPIO_NUM_4

bool SD_Config::read_wifi(WIFI_ACC_PT& ap)
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

bool SD_Config::read_players(PLAYERS& players)
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

bool SD_Config::read_favourites(FAVOURITES& favourites)
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

bool SD_Config::parse_wifi_file(File wifif, WIFI_ACC_PT& ap)
{
    bool result = false;
    tft_println("Parsing WiFi ssid/psw");
    while (wifif.available()) {
        String line = wifif.readStringUntil('\n');
        line.trim();
        DPRINT(line);
        string wifi = line.c_str();
        if (wifi.length() > 1) {
            vector<string> parts = split(wifi, '|');
            if (parts.size() == 2) {
                ap.ssid = strdup(parts[0].c_str());
                ap.psw = strdup(parts[1].c_str());
                result = true;
                break;
            }
        }
    }
    wifif.close();
    return result;
}

bool SD_Config::parse_players_file(File plf, PLAYERS& players)
{
    bool result = false;
    tft_println("Parsing players:");
    while (plf.available() && (players.size() <= 5)) { // max 5 players
        String line = plf.readStringUntil('\n');
        line.trim();
        DPRINT(line);
        string pl = line.c_str();
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

bool SD_Config::parse_favs_file(File favf, FAVOURITES& favourites)
{
    bool result = false;
    tft_println("Parsing favourites");
    while (favf.available() && favourites.size() <= 50) { // max 50
        String line = favf.readStringUntil('\n');
        line.trim();
        DPRINT(line);
        string fav = line.c_str();
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
