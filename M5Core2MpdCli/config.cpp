#include "config.h"

#include "flash_fs.h"
#include "sdcard_fs.h"
#include "tftfunctions.h"
#include "utils.h"

static CONFIG config;

void set_active_player(uint16_t new_pl)
{
    config.active_player = new_pl;
}

bool load_SD_config()
{
    tft_clear();
    tft_println("Check SD config");
    if (read_wifi_SD(config.ap)
        && read_players_SD(config.mpd_players)
        && read_favourites_SD(config.favourites)) {
        return true;
    } else {
        return false;
    }
}

bool load_FLASH_config()
{
    tft_println("Load FLASH config");
    if (read_wifi_FLASH(config.ap)
        && read_players_FLASH(config.mpd_players)
        && read_favourites_FLASH(config.favourites)
        && read_current_player()) {
        return true;
    } else {
        return false;
    }
}

bool save_FLASH_config()
{
    tft_clear();
    tft_println("Save FLASH config");
    if (write_wifi_FLASH(config.ap)
        && write_players_FLASH(config.mpd_players)
        && write_favourites_FLASH(config.favourites)) {
        return true;
    } else {
        return false;
    }
}

CONFIG& get_config()
{
    return config;
}
