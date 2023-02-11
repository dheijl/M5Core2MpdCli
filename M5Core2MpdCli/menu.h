#pragma once

typedef struct menuline {
    uint16_t x;
    uint16_t y;
    const char* text;
} MENULINE;

void show_menu();
