#pragma once

#include "config.h"

bool read_current_player();
void write_current_player(uint16_t new_pl);

bool write_wifi_FLASH(CONFIG& config);
bool write_players_FLASH(CONFIG& config);
bool write_favourites_FLASH(CONFIG& config);

bool read_wifi_FLASH(CONFIG& config);
bool read_players_FLASH(CONFIG& config);
bool read_favourites_FLASH(CONFIG& config);
