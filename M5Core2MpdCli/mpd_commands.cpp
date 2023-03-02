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

#include "mpd_commands.h"
#include "mpdcli.h"
#include "tftfunctions.h"
#include "wifi.h"

#include <M5Unified.h>

static MpdConnection con;

static void show_player(MPD_PLAYER& player)
{
    tft_println("Player: " + String(player.player_name));
}

void toggle_mpd_status()
{
    if (start_wifi()) {
        auto player = Config.get_active_mpd();
        show_player(player);
        if (con.Connect(player.player_ip, player.player_port)) {
            if (con.IsPlaying()) {
                tft_println_highlight("Stop playing");
                con.Stop();
            } else {
                tft_println_highlight("Start playing");
                con.Play();
            }
            con.Disconnect();
        }
        vTaskDelay(500);
        tft_clear();
    }
}

void show_mpd_status()
{
    auto bat_level = M5.Power.getBatteryLevel();
    auto heap = ESP.getFreeHeap() / 1024;
    auto psram = ESP.getFreePsram() / (1024 * 1024);
    tft_println("B=" + String(bat_level) + "%,H=" + String(heap) + "K,PS=" + String(psram) + "M");
    if (start_wifi()) {
        auto player = Config.get_active_mpd();
        show_player(player);
        if (con.Connect(player.player_ip, player.player_port)) {
            con.GetStatus();
            con.GetCurrentSong();
            con.Disconnect();
        }
        vTaskDelay(3000);
        tft_clear();
    }
}

void play_favourite(const FAVOURITE& fav)
{
    if (start_wifi()) {
        auto player = Config.get_active_mpd();
        show_player(player);
        tft_println_highlight("Play " + String(fav.fav_name));
        if (con.Connect(player.player_ip, player.player_port)) {
            con.Clear();
            con.Add_Url(fav.fav_url);
            con.Play();
            con.Disconnect();
        }
        vTaskDelay(1500);
        tft_clear();
    }
}
