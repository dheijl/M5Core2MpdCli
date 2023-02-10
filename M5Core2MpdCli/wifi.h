#pragma once

#include "config.h"
#include <WiFi.h>

bool is_wifi_connected();
bool start_wifi(CONFIG& config);
void stop_wifi();
