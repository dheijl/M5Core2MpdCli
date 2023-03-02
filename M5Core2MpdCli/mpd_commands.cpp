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
