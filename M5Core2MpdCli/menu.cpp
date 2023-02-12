#include "config.h"

#include "flash_fs.h"
#include "menu.h"
#include "mpd_commands.h"
#include "tftfunctions.h"

#include <M5Unified.h>

static void display_menuline(const MENULINE* line, uint16_t color)
{
    tft_write(line->x, line->y, color, String(line->text));
}

static int display_menu(const vector<MENULINE*> menu)
{
    tft_clear();
    int selected = 0;
    bool repaint = true;
    while (true) {
        if (repaint) {
            repaint = false;
            int i = 0;
            for (auto l : menu) {
                if (i++ == selected) {
                    display_menuline(l, TFT_GREENYELLOW);
                } else {
                    display_menuline(l, TFT_WHITE);
                }
            }
        }
        int down_time = 400;
        M5.update();
        if (M5.BtnC.wasPressed()) { // up
            while ((M5.BtnA.isPressed()) && (down_time-- > 0)) {
                M5.update();
            }
            selected -= 1;
            if (selected < 0) {
                selected = menu.size() - 1;
            }
            repaint = true;
            continue;
        }
        if (M5.BtnA.wasPressed()) { // down
            while ((M5.BtnC.isPressed()) && (down_time-- > 0)) {
                M5.update();
            }
            selected += 1;
            if (selected > (menu.size() - 1)) {
                selected = 0;
            }
            repaint = true;
            continue;
        }
        if (M5.BtnB.wasPressed()) { // select
            while ((M5.BtnB.isPressed()) && (down_time-- > 0)) {
                M5.update();
            }
            return selected;
        }
        if (M5.BtnPWR.wasPressed()) {
            return -1;
        }
        delay(1);
    }
}

static void select_player()
{
    auto players = get_config().mpd_players;
    vector<MENULINE*> player_menu;
    uint16_t x = 4;
    uint16_t pos = 40;
    for (auto p : players) {
        MENULINE* m = new MENULINE { x, pos, p->player_name };
        pos += 40;
        player_menu.push_back(m);
    }
    MENULINE* ret = new MENULINE { x, pos, "Return" };
    player_menu.push_back(ret);
    int selected = display_menu(player_menu);
    if ((selected >= 0) && (selected < players.size())) {
        auto pl = players[selected]->player_name;
        tft_clear();
        tft_println("New player @" + String(pl));
        write_current_player(selected);
    }
    for (auto ml = player_menu.begin(); ml != player_menu.end(); ++ml) {
        delete *ml;
    }
    player_menu.clear();
    delay(500);
}

static void select_favourite(int page)
{
    vector<MENULINE*> fav_menu;
    uint16_t x = 4;
    uint16_t pos = 15;
    int ifrom = page * 10;
    int ito = ifrom + 10;
    CONFIG& config = get_config();
    for (int i = ifrom; i < ito; i++) {
        if (i < config.favourites.size()) {
            MENULINE* m = new MENULINE { x, pos, config.favourites[i]->fav_name };
            pos += 20;
            fav_menu.push_back(m);
        }
    }
    MENULINE* ret = new MENULINE { x, pos, "Return" };
    fav_menu.push_back(ret);
    int selected = display_menu(fav_menu);
    if ((selected >= 0) && (selected < fav_menu.size() - 1)) {
        selected += ifrom;
        auto url = config.favourites[selected]->fav_url;
        auto name = config.favourites[selected]->fav_name;
        tft_clear();
        tft_println_highlight("Playing " + String(name));
        play_favourite(url);
    }
    for (auto ml = fav_menu.begin(); ml != fav_menu.end(); ++ml) {
        delete *ml;
    }
    fav_menu.clear();
}

void show_menu()
{

    static const char* mlines[] {
        "Select Player",
        "Favourites 1",
        "Favourites 2",
        "Favourites 3",
        "Favourites 4",
        "Favourites 5",
        "Return"
    };

    tft_clear();
    CONFIG& config = get_config();
    int nfavs = config.favourites.size();
    int npages = (npages % 10) == 0 ? (nfavs / 10) : (nfavs / 10) + 1;
    npages = min(npages, 5);
    vector<MENULINE*> main_menu;
    uint16_t x = 4;
    uint16_t y = 20;
    for (int i = 0; i <= npages; i++) {
        main_menu.push_back(new MENULINE { x, y, mlines[i] });
        y += 30;
    }
    main_menu.push_back(new MENULINE { x, y, mlines[6] });
    int selected = display_menu(main_menu);
    if (selected == 0) {
        select_player();
    } else if (selected <= npages) {
        select_favourite(selected - 1);
    }
    for (auto ml = main_menu.begin(); ml != main_menu.end(); ++ml) {
        delete *ml;
    }
    main_menu.clear();

    delay(500);
    tft_clear();
}
