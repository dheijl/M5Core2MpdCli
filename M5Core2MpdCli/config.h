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

#pragma once

#include <string>
#include <vector>

#include <Arduino.h>
#include <Preferences.h>
#include <SD.h>

using std::string;
using std::vector;

#undef DEBUG
// #define DEBUG
#undef DEBUGMPDMESSAGES

#ifdef DEBUG
#define DPRINT(x) Serial.println(x);
#else
#define DPRINT(x)
#endif

typedef struct wifi_acc_pt {
    const char* ssid;
    const char* psw;
} WIFI_ACC_PT;

typedef struct mpd_player {
    const char* player_name;
    const char* player_ip;
    uint16_t player_port;
} MPD_PLAYER;

typedef struct favourite {
    const char* fav_name;
    const char* fav_url;
} FAVOURITE;

typedef vector<MPD_PLAYER*> PLAYERS;
typedef vector<FAVOURITE*> FAVOURITES;

class Configuration {
private:
    uint16_t player_index;
    WIFI_ACC_PT ap;
    PLAYERS mpd_players;
    FAVOURITES favourites;
    bool load_SD_config();
    bool load_FLASH_config();
    bool save_FLASH_config();

public:
    const WIFI_ACC_PT& getAP();
    const PLAYERS& getPlayers();
    const FAVOURITES& getFavourites();
    bool load_config();
    void set_player_index(uint16_t new_pl);
    const MPD_PLAYER& get_active_mpd();
};

extern Configuration& Config;
