#pragma once

#include "config.h"

static bool parse_wifi_file(File wifif, WIFI_ACC_PT& ap);
static bool parse_players_file(File plf, vector<MPD_PLAYER*>& players);
static bool parse_favs_file(File favf, vector<FAVOURITE*>& favourites);

bool read_wifi_SD(WIFI_ACC_PT& ap);
bool read_players_SD(vector<MPD_PLAYER*>& players);
bool read_favourites_SD(vector<FAVOURITE*>& favourites);
