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

#include "flash_fs.h"

#include "tftfunctions.h"
#include "utils.h"

#include <M5Unified.h>
#include <Preferences.h>

#include <string.h>
#include <string>

static const constexpr char* NVS_WIFI = "wifi";
static const constexpr char* NVS_PLAYERS = "players";
static const constexpr char* NVS_FAVS = "favs";
static const constexpr char* NVS_CUR_MPD = "curmpd";

bool NVS_Config::write_wifi(const WIFI_ACC_PT& ap)
{
    Preferences prefs;
    bool result = false;
    if (!prefs.begin(NVS_WIFI, false)) {
        tft_println_error("wifi prefs begin error");
        prefs.end();
        vTaskDelay(2000);
        return result;
    }
    result = true;
    DPRINT("wprefs: " + String(ap.ssid) + "|" + String(ap.psw));
    result = prefs.putString("ssid", ap.ssid) > 0;
    result = prefs.putString("psw", ap.psw) > 0;
    if (!result) {
        tft_println_error("wifi prefs put error");
        vTaskDelay(2000);
    }
    prefs.end();
    return result;
}

bool NVS_Config::read_wifi(WIFI_ACC_PT& ap)
{
    Preferences prefs;
    if (!prefs.begin(NVS_WIFI, true)) {
        tft_println_error("wifi prefs begin error");
        prefs.end();
        vTaskDelay(2000);
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
    ap.ssid = strdup(ssid.c_str());
    ap.psw = strdup(psw.c_str());
    DPRINT("Wifi config: " + String(ap.ssid) + "|" + String(ap.psw));
    return true;
}

bool NVS_Config::write_players(const PLAYERS& players)
{
    Preferences prefs;
    bool result = false;
    if (!prefs.begin(NVS_PLAYERS, false)) {
        tft_println_error("players prefs begin error");
        prefs.end();
        vTaskDelay(2000);
        return result;
    }
    prefs.clear();
    result = true;
    int i = 0;
    for (auto pl : players) {
        string key = std::to_string(i);
        String data = String(pl->player_name) + "|" + String(pl->player_hostname) + "|" + String(std::to_string(pl->player_port).c_str());
        if (prefs.putString(key.c_str(), data) == 0) {
            result = false;
            tft_println_error("players prefs put error");
            vTaskDelay(2000);
            break;
        }
        DPRINT(data.c_str());
        ++i;
    }
    prefs.end();
    tft_println("Saved " + String(i) + "players");
    return result;
}

bool NVS_Config::read_players(PLAYERS& players)
{
    Preferences prefs;
    bool result = false;
    if (!prefs.begin(NVS_PLAYERS, true)) {
        tft_println_error("players prefs begin error");
        prefs.end();
        vTaskDelay(2000);
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
                mpd->player_hostname = strdup(parts[1].c_str());
                mpd->player_ip = NULL;
                mpd->player_port = stoi(parts[2]);
                players.push_back(mpd);
                tft_println(String(mpd->player_name) + " " + String(mpd->player_hostname) + ":" + String(mpd->player_port));
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

bool NVS_Config::write_favourites(const FAVOURITES& favourites)
{
    Preferences prefs;
    bool result = false;
    if (!prefs.begin("favs", false)) {
        tft_println_error("favs prefs begin error");
        prefs.end();
        vTaskDelay(2000);
        return result;
    }
    prefs.clear();
    result = true;
    int i = 0;
    for (auto f : favourites) {
        string key = std::to_string(i);
        String data = String(f->fav_name) + "|" + String(f->fav_url);
        if (prefs.putString(key.c_str(), data) == 0) {
            result = false;
            tft_println_error("favs prefs put error");
            vTaskDelay(2000);
            break;
        }
        DPRINT(data.c_str());
        ++i;
    }
    prefs.end();
    tft_println("Saved " + String(i) + "favourites");
    return result;
}

bool NVS_Config::read_favourites(FAVOURITES& favourites)
{
    Preferences prefs;
    bool result = false;
    if (!prefs.begin("favs", true)) {
        tft_println_error("favs prefs begin error");
        prefs.end();
        vTaskDelay(2000);
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
        vector<string> parts = split(string(fav.c_str()), '|');
        if (parts.size() == 2) {
            FAVOURITE* f = new FAVOURITE();
            f->fav_name = strdup(parts[0].c_str());
            f->fav_url = strdup(parts[1].c_str());
            favourites.push_back(f);
        }
        ++i;
    }
    if (i > 0) {
        result = true;
    }
    prefs.end();
    tft_println_highlight("Loaded " + String(favourites.size()) + " favourites");
    return result;
}

void NVS_Config::write_player_index(uint16_t new_pl)
{
    Preferences prefs;
    if (!prefs.begin(NVS_CUR_MPD, false)) {
        tft_println_error("cur_mpd prefs begin error");
        prefs.end();
        vTaskDelay(2000);
        return;
    }
    DPRINT("wprefs player: " + String(new_pl));
    Config.set_player_index(new_pl);
    bool result = prefs.putUShort("cur_mpd", new_pl) > 0;
    if (!result) {
        tft_println_error("cur_mpd prefs put error");
        vTaskDelay(2000);
    }
    prefs.end();
}

bool NVS_Config::read_player_index()
{
    Preferences prefs;
    if (!prefs.begin(NVS_CUR_MPD, true)) {
        prefs.end();
        tft_println_highlight("No cur_mpd prefs!");
        write_player_index(0);
        Config.set_player_index(0);
        return true;
    }
    bool result = false;
    int cur_mpd = prefs.getUShort("cur_mpd", 999);
    DPRINT("cur_mpd = " + String(cur_mpd));
    if (cur_mpd != 999) {
        Config.set_player_index(cur_mpd);
        result = true;
    }
    prefs.end();
    return result;
}
