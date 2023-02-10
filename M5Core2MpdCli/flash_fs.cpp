#include "flash_fs.h"
#include "config.h"
#include "tftfunctions.h"

bool write_wifi_FLASH(CONFIG& config)
{ /*
bool result = false;
if (!SFUD.begin(104000000UL)) {
tft_println("FLASH mount failed");
return result;
}
if (SFUD.remove("wifi.txt")) {
tft_println("wifi.txt removed");
}
String data = String(config.ssid) + "|" + String(config.psw);
File wifif = SFUD.open("wifi.txt", FILE_WRITE);
if (wifif) {
wifif.println(data);
DPRINT(data.c_str());
wifif.close();
result = true;
} else {
tft_println_error("Error writing wifi.txt");
}
SFUD.end();
return result;
*/
    return true;
}

bool write_player_FLASH(CONFIG& config)
{ /*
    bool result = false;
    if (!SFUD.begin(104000000UL)) {
        tft_println_error("FLASH mount failed");
        return result;
    }
    if (SFUD.remove("players.txt")) {
        tft_println("players.txt removed");
    }
    File plf = SFUD.open("players.txt", FILE_WRITE);
    if (plf) {
        for (auto pl : config.mpd_players) {
            String data = String(pl->player_name) + "|" + String(pl->player_ip) + "|" + String(to_string(pl->player_port).c_str());
            plf.println(data);
            DPRINT(data.c_str());
        }
        plf.close();
        result = true;
    } else {
        tft_println_error("Error writing wifi.txt");
    }
    SFUD.end();
    return result;
    */
    return true;
}

bool write_favourites_FLASH(CONFIG& config)
{ /*
    bool result = false;
    if (!SFUD.begin(104000000UL)) {
        tft_println_error("FLASH mount failed");
        return result;
    }
    if (SFUD.remove("favs.txt")) {
        tft_println("favs.txt removed");
    }
    File favf = SFUD.open("favs.txt", FILE_WRITE);
    if (favf) {
        for (auto f : config.favourites) {
            String data = String(f->fav_name) + "|" + String(f->fav_url);
            favf.println(data);
            DPRINT(data.c_str());
        }
        favf.close();
        result = true;
    } else {
        tft_println_error("Error writing favs.txt");
    }
    SFUD.end();
    return result;
    */
    return true;
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
