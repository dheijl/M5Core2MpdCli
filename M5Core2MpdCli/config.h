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
