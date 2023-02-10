#pragma once

#include "config.h"

#include "Free_Fonts.h" //include the header file
#include "M5Unified.h"

void init_tft();
void tft_print(const String s);
void tft_write(int16_t x, int16_t y, uint16_t color, const String s);
void tft_write_error(int16_t x, int16_t y, const String s);
void tft_write_highlight(int16_t x, int16_t y, const String s);
void tft_println(const String s, uint16_t color = TFT_WHITE);
void tft_println_error(const String s);
void tft_println_highlight(const String s);
void tft_clear();
