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

#include "Free_Fonts.h"

#include "tftfunctions.h"

static bool sleeping = false;

void init_tft()
{
    M5.Display.begin();
    M5.Display.setRotation(3);
    M5.Display.setFont(FM9);
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setTextSize(1);
}

void tft_write(int16_t x, int16_t y, uint16_t color, const String& s)
{
    if (sleeping) {
        sleeping = false;
        M5.Display.wakeup();
    }
    M5.Display.setCursor(x, y);
    M5.Display.setTextColor(color);
    M5.Display.print(s.c_str());
    DPRINT(String(x) + ", " + String(y) + ", " + String(color) + ", " + s);
}

void tft_write_error(int16_t x, int16_t y, const String& s)
{
    tft_write(x, y, TFT_RED, s);
}

void tft_write_highlight(int16_t x, int16_t y, const String& s)
{
    tft_write(x, y, TFT_GREENYELLOW, s);
}

void tft_println(const String& s, uint16_t color)
{
    if (sleeping) {
        sleeping = false;
        M5.Display.wakeup();
    }
    M5.Display.setTextColor(color);
    M5.Display.println(s);
    DPRINT(s);
}

void tft_println_error(const String& s)
{
    tft_println(s, TFT_RED);
}

void tft_println_highlight(const String& s)
{
    tft_println(s, TFT_GREENYELLOW);
}

void tft_print(const String& s)
{
    if (sleeping) {
        sleeping = false;
        M5.Display.wakeup();
    }
    M5.Display.print(s);
}

void tft_clear()
{
    if (sleeping) {
        sleeping = false;
        M5.Display.wakeup();
    }
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setCursor(0, 0);
    M5.Display.println();
}

void tft_sleep()
{
    M5.Display.sleep();
    sleeping = true;
}
