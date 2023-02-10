#pragma once

#include "config.h"
#include "flash_fs.h"
#include "mpd_commands.h"
#include "tftfunctions.h"

using namespace std;

typedef struct menuline {
  uint16_t x;
  uint16_t y;
  const char* text;
} MENULINE;

void show_menu();
