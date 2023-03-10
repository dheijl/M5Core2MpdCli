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

#include "config.h"

#include "flash_fs.h"
#include "sdcard_fs.h"
#include "tftfunctions.h"
#include "utils.h"

static Configuration config;

Configuration& Config = config;

///
/// try to load a config from SD or from flash
///
bool Configuration::load_config()
{
    if (this->load_SD_config()) {
        if (this->save_FLASH_config()) {
            tft_println("Configuration saved to FLASH");
        } else {
            tft_println_error("Error saving to FLASH");
        }
    } else {
        if (!this->load_FLASH_config()) {
            return false;
        }
    }
    return true;
}

void Configuration::set_player_index(uint16_t new_pl)
{
    this->player_index = new_pl;
}

const MPD_PLAYER& Configuration::get_active_mpd()
{
    return *(this->mpd_players[config.player_index]);
}

const WIFI_ACC_PT& Configuration::getAP()
{
    return this->ap;
}

const PLAYERS& Configuration::getPlayers()
{
    return this->mpd_players;
}

const FAVOURITES& Configuration::getFavourites()
{
    return this->favourites;
}

bool Configuration::load_SD_config()
{
    tft_clear();
    tft_println("Check SD config");
    if (read_wifi_SD(this->ap)
        && read_players_SD(this->mpd_players)
        && read_favourites_SD(this->favourites)) {
        return true;
    } else {
        return false;
    }
}

bool Configuration::load_FLASH_config()
{
    tft_println("Load FLASH config");
    if (read_wifi_FLASH(this->ap)
        && read_players_FLASH(this->mpd_players)
        && read_favourites_FLASH(this->favourites)
        && read_player_index()) {
        return true;
    } else {
        return false;
    }
}

bool Configuration::save_FLASH_config()
{
    tft_clear();
    tft_println("Save FLASH config");
    if (write_wifi_FLASH(this->ap)
        && write_players_FLASH(this->mpd_players)
        && write_favourites_FLASH(this->favourites)) {
        return true;
    } else {
        return false;
    }
}
