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

typedef struct config {
    WIFI_ACC_PT ap;
    uint16_t active_player;
    vector<MPD_PLAYER*> mpd_players;
    vector<FAVOURITE*> favourites;
} CONFIG;

void set_active_player(uint16_t new_pl);

bool load_SD_config();
bool load_FLASH_config();
bool save_FLASH_config();
CONFIG& get_config();
