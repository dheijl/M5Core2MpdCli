#include "battery.h"
#include "config.h"
#include "tftfunctions.h"

const unsigned int BATTERY_CAPACITY = 650; // Set Wio Terminal Battery's Capacity

static bool _have_battery = false;

bool init_battery(void)
{ /*
// Use lipo.begin() to initialize the BQ27441-G1A and confirm that it's connected and communicating.
if (!lipo.begin())  // begin() will return true if communication is successful
{
// If communication fails, print an error message and loop forever.
DPRINT("Error: Unable to communicate with BQ27441.");
tft_println_error("Battery Not Initialised!");
return false;
}
DPRINT("Connected to BQ27441!");
// Uset lipo.setCapacity(BATTERY_CAPACITY) to set the design capacity
// of your battery.
lipo.setCapacity(BATTERY_CAPACITY);
_have_battery = true;
*/
    return true;
}

void printBatteryStats()
{ /*
if (!_have_battery) {
return;
}
tft_clear();

// Read battery stats from the BQ27441-G1A
unsigned int soc = lipo.soc(FILTERED);            // Read state-of-charge (%)
unsigned int volts = lipo.voltage();              // Read battery voltage (mV)
int current = lipo.current(AVG);                  // Read average current (mA)
unsigned int fullCapacity = lipo.capacity(FULL);  // Read full capacity (mAh)
unsigned int capacity = lipo.capacity(REMAIN);    // Read remaining capacity (mAh)
int power = lipo.power();                         // Read average power draw (mW)
int health = lipo.soh(PERCENT);                   // Read state-of-health (%)

#ifdef DEBUG
// Now print out those values:
String toPrint = String(soc) + "% | ";
toPrint += String(volts) + " mV | ";
toPrint += String(current) + " mA | ";
toPrint += String(capacity) + " / ";
toPrint += String(fullCapacity) + " mAh | ";
toPrint += String(power) + " mW | ";
toPrint += String(health) + "%";

DPRINT(toPrint);
#endif

// LCD Graphics
tft_println("Charge: " + String(soc) + " %");
tft_println("Voltage: " + String(volts) + " mV");
tft_println("Avg Current :" + String(current) + " mA");
tft_println("Rem Cap :" + String(capacity) + " mAH");
tft_println("Full Cap :" + String(fullCapacity) + " mAH");
tft_println("Avg Power: " + String(power) + " mW");
tft_println("Health: " + String(health) + " %");

while (digitalRead(WIO_KEY_B) == LOW) {}

tft_clear();
*/
}
