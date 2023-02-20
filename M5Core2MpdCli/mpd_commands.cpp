#include "config.h"

#include "mpd_commands.h"
#include "mpdcli.h"
#include "tftfunctions.h"
#include "wifi.h"

#include <M5Unified.h>

static MPD_PLAYER* get_mpd()
{
    auto config = get_config();
    if (start_wifi(config)) {
        MPD_PLAYER* mpd = config.mpd_players[config.active_player];
        tft_println("Player: " + String(config.active_player) + " " + String(mpd->player_name));
        return mpd;
    } else {
        tft_println_error("Can't connect.");
        return NULL;
    }
}

static void disconnect_mpd()
{
    tft_clear();
}

void toggle_mpd_status()
{
    auto player = get_mpd();
    if (player != NULL) {
        MpdConnection con;
        if (con.Connect(player->player_ip, player->player_port)) {
            if (con.IsPlaying()) {
                tft_println_highlight("Stop playing");
                con.Stop();
            } else {
                tft_println_highlight("Start playing");
                con.Play();
            }
            con.Disconnect();
        }
        disconnect_mpd();
    }
}

void show_mpd_status()
{
    int bat_level = M5.Power.getBatteryLevel();
    tft_println("Battery: " + String(std::to_string(bat_level).c_str()));
    auto player = get_mpd();
    if (player != NULL) {
        MpdConnection con;
        if (con.Connect(player->player_ip, player->player_port)) {
            con.GetStatus();
            con.GetCurrentSong();
            con.Disconnect();
        }
        vTaskDelay(3000);
        disconnect_mpd();
    }
}

void play_favourite(const char* url)
{
    auto player = get_mpd();
    if (player != NULL) {
        MpdConnection con;
        if (con.Connect(player->player_ip, player->player_port)) {
            con.Clear();
            con.Add_Url(url);
            con.Play();
        }
        vTaskDelay(2000);
        disconnect_mpd();
    }
}
