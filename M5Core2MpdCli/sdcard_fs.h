#pragma once

#include "config.h"

bool read_wifi_SD(WIFI_ACC_PT& ap);
bool read_players_SD(vector<MPD_PLAYER*>& players);
bool read_favourites_SD(vector<FAVOURITE*>& favourites);
