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

#include "flash_fs.h"
#include "menu.h"
#include "mpd_commands.h"
#include "tftfunctions.h"

#include <M5Unified.h>

typedef vector<MENULINE*> MENU;

static void draw_menuline(const MENULINE* line, uint16_t color)
{
    tft_write(line->x, line->y, color, String(line->text));
}

static void draw(const MENU& menu, const int selected)
{
    int i = 0;
    for (auto l : menu) {
        if (i++ == selected) {
            draw_menuline(l, TFT_GREENYELLOW);
        } else {
            draw_menuline(l, TFT_WHITE);
        }
    }
}

static int display_menu(const MENU& menu)
{
    tft_clear();
    int selected = 0;
    bool repaint = true;
    while (true) {
        vTaskDelay(5);
        if (repaint) {
            repaint = false;
            draw(menu, selected);
        }
        M5.update();
        if (M5.BtnC.wasPressed()) { // up
            selected -= 1;
            if (selected < 0) {
                selected = menu.size() - 1;
            }
            repaint = true;
            continue;
        }
        if (M5.BtnA.wasPressed()) { // down
            selected += 1;
            if (selected > (menu.size() - 1)) {
                selected = 0;
            }
            repaint = true;
            continue;
        }
        if (M5.BtnB.wasPressed()) { // select
            return selected;
        }
        // select with touch
        M5.update();
        auto count = M5.Touch.getCount();
        if (count > 0) {
            for (uint i = 0; i < count; ++i) {
                auto det = M5.Touch.getDetail(i);
                if (det.isPressed()) {
                    int sel = 0;
                    for (auto ml : menu) {
                        if ((det.y >= ml->y) && (det.y <= ml->y + 12)) {
                            selected = sel;
                            repaint = true;
                            continue;
                        }
                        ++sel;
                    }
                }
            }
        }
    }
}

static void select_player()
{
    auto players = Config.getPlayers();
    MENU player_menu;
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
        Config.set_player_index((uint16_t)selected);
        write_player_index(selected);
    }
    for (auto ml = player_menu.begin(); ml != player_menu.end(); ++ml) {
        delete *ml;
    }
    player_menu.clear();
}

static void select_favourite(int page)
{
    auto favs = Config.getFavourites();
    MENU fav_menu;
    uint16_t x = 4;
    uint16_t pos = 15;
    int ifrom = page * 10;
    int ito = ifrom + 10;
    for (int i = ifrom; i < ito; i++) {
        if (i < favs.size()) {
            MENULINE* m = new MENULINE { x, pos, favs[i]->fav_name };
            pos += 20;
            fav_menu.push_back(m);
        }
    }
    MENULINE* ret = new MENULINE { x, pos, "Return" };
    fav_menu.push_back(ret);
    int selected = display_menu(fav_menu);
    if ((selected >= 0) && (selected < fav_menu.size() - 1)) {
        selected += ifrom;
        FAVOURITE& fav = *favs[selected];
        tft_clear();
        play_favourite(fav);
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
    int nfavs = Config.getFavourites().size();
    int npages = (nfavs % 10) == 0 ? (nfavs / 10) : (nfavs / 10) + 1;
    npages = min(npages, 5);
    MENU main_menu;
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
    tft_clear();
}
