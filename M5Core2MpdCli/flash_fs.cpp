#include "flash_fs.h"

#include "tftfunctions.h"

#include <M5Unified.h>
#include <Preferences.h>

const char* NVS_WIFI = "wifi";
const char* NVS_PLAYERS = "players";
const char* NVS_FAVS = "favs";
const char* NVS_CUR_MPD = "curmpd";

bool write_wifi_FLASH(CONFIG& config)
{
    Preferences prefs;
    bool result = false;
    if (!prefs.begin(NVS_WIFI, false)) {
        tft_println_error("wifi prefs begin error");
        prefs.end();
        delay(1000);
        return result;
    }
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

bool read_wifi_FLASH(CONFIG& config)
{
    Preferences prefs;
    if (!prefs.begin(NVS_WIFI, true)) {
        tft_println_error("wifi prefs begin error");
        prefs.end();
        delay(1000);
        return false;
    }
    String ssid = prefs.getString("ssid");
    String psw = prefs.getString("psw");
    prefs.end();
    DPRINT(ssid + "|" + psw);
    if (ssid.isEmpty() || psw.isEmpty()) {
      tft_println_error("empty wifi prefs!");
      return false;
    }
    config.ssid = strdup(ssid.c_str());
    config.psw = strdup(psw.c_str());
    DPRINT("Wifi config: " + String(config.ssid) + "|" + String(config.psw));
    return true;
}

bool write_players_FLASH(CONFIG& config)
{
    Preferences prefs;
    bool result = false;
    if (!prefs.begin(NVS_PLAYERS, false)) {
        tft_println_error("players prefs begin error");
        prefs.end();
        delay(1000);
        return result;
    }
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
    tft_println("Saved " + String(i) + "players");
    return result;
}

bool read_players_FLASH(CONFIG& config)
{
    Preferences prefs;
    bool result = false;
    if (!prefs.begin(NVS_PLAYERS, true)) {
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
    tft_println_highlight("Loaded " + String(i) + " players");
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
    tft_println("Saved " + String(i) + "favourites");
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
    tft_println_highlight("Loaded " + String(i) + " favourites");
    return result;
}

void write_current_player(int new_pl)
{
    Preferences prefs;
    auto config = get_config();
    config.active_player = (uint16_t)new_pl;
    if (!prefs.begin(NVS_CUR_MPD, false)) {
        tft_println_error("cur_mpd prefs begin error");
        prefs.end();
        delay(1000);
        return;
    }
    DPRINT("wprefs player: " + String(new_pl));
    bool result = prefs.putUShort("cur_mpd", (uint16_t)new_pl) > 0;
    if (!result) {
        tft_println_error("cur_mpd prefs put error");
        delay(1000);
    }
    prefs.end();
}

bool read_current_player(CONFIG& config)
{
    Preferences prefs;
    if (!prefs.begin(NVS_CUR_MPD, true)) {
        prefs.end();
        tft_println_highlight("No cur_mpd prefs!");
        write_current_player(0);
        return true;
    }
    bool result = false;
    int cur_mpd = prefs.getUShort("cur_mpd", 999);
    DPRINT("cur_mpd = " + String(cur_mpd));
    if (cur_mpd != 999) {
        result = true;
        config.active_player = cur_mpd;
    }
    prefs.end();
    return result;
}
