#pragma once

#include "config.h"

bool read_player_index();
void write_player_index(uint16_t new_pl);

bool write_wifi_FLASH(const WIFI_ACC_PT& ap);
bool write_players_FLASH(const PLAYERS& players);
bool write_favourites_FLASH(const FAVOURITES& favourites);

bool read_wifi_FLASH(WIFI_ACC_PT& ap);
bool read_players_FLASH(PLAYERS& players);
bool read_favourites_FLASH(FAVOURITES& favourites);
