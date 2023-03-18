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

#include "menu.h"
#include "flash_fs.h"
#include "mpdcli.h"

#include <M5Unified.h>

void Menu::select_player()
{
    auto players = Config.getPlayers();
    int selected = this->PlayerMenu.display_menu();
    if ((selected >= 0) && (selected < players.size())) {
        auto pl = players[selected]->player_name;
        tft_clear();
        tft_println("New player @" + String(pl));
        Config.set_player_index((uint16_t)selected);
        NVS_Config::write_player_index(selected);
    }
}

void Menu::select_favourite(int page)
{
    auto favs = Config.getFavourites();
    auto fav_menu = this->FavouriteMenus[page];
    int selected = fav_menu->display_menu();
    if ((selected >= 0) && (selected < fav_menu->size() - 1)) {
        selected = (page * Menu::MAXLINES) + selected;
        FAVOURITE& fav = *favs[selected];
        tft_clear();
        mpd.play_favourite(fav);
    }
}

void Menu::Show()
{
    tft_clear();
    int nfavs = Config.getFavourites().size();
    int npages = (nfavs % Menu::MAXLINES) == 0 ? (nfavs / Menu::MAXLINES) : (nfavs / Menu::MAXLINES) + 1;
    int selected = this->MainMenu.display_menu();
    if (selected == 0) {
        this->select_player();
    } else if (selected <= npages) {
        this->select_favourite(selected - 1);
    }
    tft_clear();
}

void Menu::CreateMenus()
{
    // main menu
    DPRINT("Creating MAIN menu");
    static constexpr char* mlines[] {
        "Select Player",
        "Favourites 1",
        "Favourites 2",
        "Favourites 3",
        "Favourites 4",
        "Favourites 5",
        "Return"
    };
    auto favs = Config.getFavourites();
    int nfavs = favs.size();
    int npages = (nfavs % Menu::MAXLINES) == 0 ? (nfavs / Menu::MAXLINES) : (nfavs / Menu::MAXLINES) + 1;
    npages = min(npages, 5);
    this->MainMenu.reserve(npages + 2);
    for (int i = 0; i <= npages; i++) {
        this->MainMenu.add_line(mlines[i]);
    }
    this->MainMenu.add_line(mlines[6]);
    DPRINT("Main menu lines: " + String(MainMenu.size()));
    // player menu
    DPRINT("Creating PLAYER menu");
    auto players = Config.getPlayers();
    this->PlayerMenu.reserve(players.size() + 1);
    for (auto p : players) {
        this->PlayerMenu.add_line(p->player_name);
    }
    this->PlayerMenu.add_line("Return");
    DPRINT("Player menu lines: " + String(MainMenu.size()));
    // favourites Menus
    this->FavouriteMenus.reserve(npages + 1);
    for (int page = 0; page < npages; ++page) {
        DPRINT("Creating FAVOURITES menu " + String(page));
        auto favmenu = new SubMenu(20);
        favmenu->reserve(11);
        int ifrom = page * Menu::MAXLINES;
        int ito = ifrom + Menu::MAXLINES;
        ito = min(nfavs, ito);
        for (int i = ifrom; i < ito; i++) {
            DPRINT("FAV#" + String(i) + ":" + String(favs[i]->fav_name));
            favmenu->add_line(favs[i]->fav_name);
        }
        favmenu->add_line("Return");
        this->FavouriteMenus.push_back(favmenu);
        DPRINT("Favourites menu " + String(page) + " lines: " + String(favmenu->size()));
    }
    DPRINT("Menus created");
}
