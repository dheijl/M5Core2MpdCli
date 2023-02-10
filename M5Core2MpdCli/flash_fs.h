#pragma once

#include "config.h"

bool read_current_player(CONFIG& config);
void write_current_player(CONFIG& config, int new_pl);

bool write_wifi_FLASH(CONFIG& config);
bool write_player_FLASH(CONFIG& config);
bool write_favourites_FLASH(CONFIG& config);

bool read_wifi_FLASH(CONFIG& config);
bool read_players_FLASH(CONFIG& config);
bool read_favourites_FLASH(CONFIG& config);
