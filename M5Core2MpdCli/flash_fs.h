#pragma once

#include "config.h"

bool read_current_player();
void write_current_player(uint16_t new_pl);

bool write_wifi_FLASH(WIFI_ACC_PT& ap);
bool write_players_FLASH(vector<MPD_PLAYER*>& players);
bool write_favourites_FLASH(vector<FAVOURITE*>& favourites);

bool read_wifi_FLASH(WIFI_ACC_PT& ap);
bool read_players_FLASH(vector<MPD_PLAYER*>& players);
bool read_favourites_FLASH(vector<FAVOURITE*>& favourites);
