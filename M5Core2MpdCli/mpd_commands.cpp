#include "config.h"

#include "mpd_commands.h"
#include "mpdcli.h"
#include "tftfunctions.h"
#include "wifi.h"

#include <M5Unified.h>

static void show_player(MPD_PLAYER& player)
{
    tft_println("Player: " + String(player.player_name));
}

void toggle_mpd_status()
{
    if (start_wifi()) {
        auto player = get_active_mpd();
        show_player(player);
        MpdConnection con;
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
        auto player = get_active_mpd();
        show_player(player);
        MpdConnection con;
        if (con.Connect(player.player_ip, player.player_port)) {
            con.GetStatus();
            con.GetCurrentSong();
            con.Disconnect();
        }
        vTaskDelay(3000);
        tft_clear();
    }
}

void play_favourite(const char* url)
{
    if (start_wifi()) {
        auto player = get_active_mpd();
        show_player(player);
        MpdConnection con;
        if (con.Connect(player.player_ip, player.player_port)) {
            con.Clear();
            con.Add_Url(url);
            con.Play();
        }
        vTaskDelay(2000);
        tft_clear();
    }
}
