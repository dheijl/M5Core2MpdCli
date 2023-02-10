#pragma once

#include <string>
#include <vector>

#include <Arduino.h>
#include <SD.h>
#include <Preferences.h>

using namespace std;

//#undef DEBUG
#define DEBUG
#undef DEBUGMPDMESSAGES

#ifdef DEBUG
#define DPRINT(x) Serial.println(x);
#else
#define DPRINT(x)
#endif

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
  const char* ssid;
  const char* psw;
  uint16_t active_player;
  vector<MPD_PLAYER*> mpd_players;
  vector<FAVOURITE*> favourites;
} CONFIG;

vector<string> split(const string& s, char delim);

bool parse_wifi_file(File wifif, CONFIG& config);
bool parse_players_file(File plf, CONFIG& config);
bool parse_favs_file(File favf, CONFIG& config);

bool load_SD_config();
bool load_FLASH_config();
bool save_FLASH_config();
CONFIG& get_config();
