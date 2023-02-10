#include "mpd_commands.h"
#include <M5Unified.h>

static string MPD_HOST = "";
static const int MPD_PORT = 6600;

static MPD_PLAYER* get_mpd()
{
    if (start_wifi(get_config())) {
        CONFIG& config = get_config();
        MPD_PLAYER* mpd = config.mpd_players[config.active_player];
        tft_println("Player: " + String(mpd->player_ip));
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
    auto player = get_mpd();
    if (player != NULL) {
        MpdConnection con;
        if (con.Connect(player->player_ip, player->player_port)) {
            con.GetStatus();
            con.GetCurrentSong();
            con.Disconnect();
        }
        delay(3000);
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
        delay(2000);
        disconnect_mpd();
    }
}
