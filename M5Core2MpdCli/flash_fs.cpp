#include "flash_fs.h"

#include "tftfunctions.h"

#include <M5Unified.h>
#include <Preferences.h>

static Preferences prefs;

bool write_wifi_FLASH(CONFIG& config)
{
    bool result = false;
    if (!prefs.begin("wifi", false)) {
        tft_println_error("wifi prefs begin error");
        prefs.end();
        delay(1000);
        return result;
    }
    result = true;
    result = prefs.putString("ssid", config.ssid) > 0;
    result = prefs.putString("psw", config.psw) > 0;
    if (!result) {
        tft_println_error("wifi prefs put error");
        delay(1000);
    }
    prefs.end();
    return result;
}

bool write_player_FLASH(CONFIG& config)
{
    bool result = false;
    if (!prefs.begin("players", false)) {
        tft_println_error("players prefs begin error");
        prefs.end();
        delay(1000);
        return result;
    }
    result = true;
    int i = 0;
    for (auto pl : config.mpd_players) {
        String data = String(pl->player_name) + "|" + String(pl->player_ip) + "|" + String(to_string(pl->player_port).c_str());
        if (prefs.putString(std::to_string(i).c_str(), data) == 0) {
            result = false;
            tft_println_error("players prefs put error");
            delay(1000);
            break;
        }
        DPRINT(data.c_str());
        ++i;
    }
    prefs.end();
    return result;
}

bool write_favourites_FLASH(CONFIG& config)
{
    bool result = false;
    if (!prefs.begin("favs", false)) {
        tft_println_error("favs prefs begin error");
        prefs.end();
        delay(1000);
        return result;
    }
    result = true;
    int i = 0;
    for (auto f : config.favourites) {
        String data = String(f->fav_name) + "|" + String(f->fav_url);
        if (prefs.putString(std::to_string(i).c_str(), data) == 0) {
            result = false;
            tft_println_error("favs prefs put error");
            delay(1000);
            break;
        }
        DPRINT(data.c_str());
        ++i;
    }
    prefs.end();
    return result;
}

bool read_wifi_FLASH(CONFIG& config)
{
    /*
      bool result = false;
      if (!SFUD.begin(104000000UL)) {
          tft_println_error("FLASH mount failed");
          return result;
      }
      File wifif = SFUD.open("wifi.txt", FILE_READ);
      if (wifif) {
          result = parse_wifi_file(wifif, config);
      }
      SFUD.end();
      return result;
      */
    return true;
}

bool read_players_FLASH(CONFIG& config)
{
    /*
      bool result = false;
      if (!SFUD.begin(104000000UL)) {
          tft_println_error("FLASH mount failed");
          return result;
      }
      File plf = SFUD.open("players.txt", FILE_READ);
      if (plf) {
          result = parse_players_file(plf, config);
      }
      SFUD.end();
      return result;
      */
    return true;
}

bool read_favourites_FLASH(CONFIG& config)
{
    /*
       bool result = false;
      if (!SFUD.begin(104000000UL)) {
          tft_println_error("FLASH mount failed");
          return result;
      }
      File favf = SFUD.open("favs.txt", FILE_READ);
      if (favf) {
          result = parse_favs_file(favf, config);
      }
      SFUD.end();
      return result;
      */
    return true;
}

void write_current_player(CONFIG& config, int new_pl)
{
    /*
      if (!SFUD.begin(104000000UL)) {
          tft_println_error("FLASH mount failed");
          return;
      }
      tft_println("FLASH mounted");
      File ipfile = SFUD.open("curmpd.txt", FILE_WRITE);
      if (ipfile) {
          ipfile.write((byte)(new_pl & 0x0ff));
          ipfile.close();
          tft_println_highlight("Saved player " + String(new_pl));
          config.active_player = new_pl;
      } else {
          tft_println_error("Can't write curmpd.txt");
      }
      SFUD.end();
      */
}

bool read_current_player(CONFIG& config)
{
    /*
      if (!SFUD.begin(104000000UL)) {
          tft_println_error("FLASH mount failed");
          return false;
      }
      File current_mpd = SFUD.open("curmpd.txt", FILE_READ);
      if (current_mpd) {
          int current = 0;
          if (current_mpd.available()) {
              current = (int)current_mpd.read();
          }
          current_mpd.close();
          SFUD.end();
          config.active_player = current;
          return true;
      } else {
          write_current_player(config, 0);
          return true;
      }
      */
    return true;
}
