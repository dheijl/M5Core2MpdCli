#include "flash_fs.h"

#include "tftfunctions.h"

#include <M5Unified.h>
#include <Preferences.h>


bool write_wifi_FLASH(CONFIG& config)
{
    Preferences prefs;
    bool result = false;
    if (!prefs.begin("wifi", false)) {
        tft_println_error("wifi prefs begin error");
        prefs.end();
        delay(1000);
        return result;
    }
    prefs.clear();
    result = true;
     DPRINT("wprefs: " + String(config.ssid) + "|" + String(config.psw));
    result = prefs.putString("ssid", config.ssid) > 0;
    result = prefs.putString("psw", config.psw) > 0;
    if (!result) {
        tft_println_error("wifi prefs put error");
        delay(1000);
    }
    prefs.end();
    return result;
}

bool write_players_FLASH(CONFIG& config)
{
    Preferences prefs;
    bool result = false;
    if (!prefs.begin("players", false)) {
        tft_println_error("players prefs begin error");
        prefs.end();
        delay(1000);
        return result;
    }
    prefs.clear();
    result = true;
    int i = 0;
    for (auto pl : config.mpd_players) {
        string key = std::to_string(i);
        String data = String(pl->player_name) + "|" + String(pl->player_ip) + "|" + String(to_string(pl->player_port).c_str());
        if (prefs.putString(key.c_str(), data) == 0) {
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
    Preferences prefs;
    bool result = false;
    if (!prefs.begin("favs", false)) {
        tft_println_error("favs prefs begin error");
        prefs.end();
        delay(1000);
        return result;
    }
    prefs.clear();
    result = true;
    int i = 0;
    for (auto f : config.favourites) {
        string key = std::to_string(i);
        String data = String(f->fav_name) + "|" + String(f->fav_url);
        if (prefs.putString(key.c_str(), data) == 0) {
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
    Preferences prefs;
    bool result = false;
    if (!prefs.begin("favs", true)) {
        tft_println_error("favs prefs begin error");
        prefs.end();
        delay(1000);
        return result;
    }
    String ssid = prefs.getString("ssid");
    String psw = prefs.getString("psw");
    DPRINT(ssid + "|" + psw);
    if (!(ssid.isEmpty() && psw.isEmpty())) {
        result = true;
        auto config = get_config();
        config.ssid = strdup(ssid.c_str());
        config.psw = strdup(psw.c_str());
    }
    prefs.end();
    return result;
}

bool read_players_FLASH(CONFIG& config)
{
    Preferences prefs;
    bool result = false;
    if (!prefs.begin("players", true)) {
        tft_println_error("players prefs begin error");
        prefs.end();
        delay(1000);
        return result;
    }
    int i = 0;
    while (true) {
        string key = std::to_string(i);
        String pl = prefs.getString(key.c_str());
        if (pl.isEmpty()) {
            break;
        } else {
            vector<string> parts = split(string(pl.c_str()), '|');
            if (parts.size() == 3) {
                auto mpd = new MPD_PLAYER();
                mpd->player_name = strdup(parts[0].c_str());
                mpd->player_ip = strdup(parts[1].c_str());
                mpd->player_port = stoi(parts[2]);
                config.mpd_players.push_back(mpd);
                tft_println(String(mpd->player_name) + " " + String(mpd->player_ip) + ":" + String(mpd->player_port));
            }
        }
        DPRINT(pl);
        ++i;
    }
    if (i > 0) {
        result = true;
    }
    prefs.end();
    tft_println_highlight("Loaded " + String(i) + "players");
    return result;
}

bool read_favourites_FLASH(CONFIG& config)
{
    Preferences prefs;
    bool result = false;
    if (!prefs.begin("favs", true)) {
        tft_println_error("favs prefs begin error");
        prefs.end();
        delay(1000);
        return result;
    }
    int i = 0;
    while (true) {
        string key = std::to_string(i);
        String fav = prefs.getString(key.c_str());
        if (fav.isEmpty()) {
            break;
        }
        DPRINT(fav);
        auto f = new FAVOURITE();
        vector<string> parts = split(string(fav.c_str()), '|');
        if (parts.size() == 2) {
            FAVOURITE* f = new FAVOURITE();
            f->fav_name = strdup(parts[0].c_str());
            f->fav_url = strdup(parts[1].c_str());
            config.favourites.push_back(f);
        }
        ++i;
    }
    if (i > 0) {
        result = true;
    }
    prefs.end();
    tft_println_highlight("Loaded " + String(i) + "favs");
    return result;
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
