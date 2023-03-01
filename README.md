# M5Core2MpdCli

Simple MPD client (Arduino) with the [M5stack M5Core2](https://docs.m5stack.com/en/core/core2).

It is the M5Core2 version of my no longer maintained [WiotMpdcli project](https://github.com/dheijl/WiotMpdcli), with similar function and use.

The joystick function of the Seeed Wio Terminal is replaced by the 3 on-screen touch buttons (up/select/down), and the menu is activated with the middle (select) button.

## Operation

Powering on or pushing Key_A will display the current MPD status (play/stop, current file, current artist/song), or move the selection UP in the menu

![01](https://user-images.githubusercontent.com/2384545/222172841-3ff9ac18-f94e-4c4e-9cc0-97e544568470.jpg)

Pushing KEY_B will show the menu, or select a menu option.

Pushing KEY_C acts as a toggle for starting/stopping the MPD player, or move the selection DOWN in the menu.

You can also use the touch screen to highlight a menu option, confirm the selection with KEY_B.

The menu system that allows you to:

- select a player from a list of players (max 5 players supported)
- select a favourite page from a list of favourites pages (max 50 favourites, so max 5 pages of 10 favourites each). You can then select one of the 10 favourites in that page.

![02](https://user-images.githubusercontent.com/2384545/222172863-71fc3b46-54e5-4cc8-a4c9-092ba327fe20.jpg)

![03](https://user-images.githubusercontent.com/2384545/222172920-af360d83-22eb-4cef-9bcc-0dad5d4a0aec.jpg)

## configuration

The currently active MPD player is stored in the ESP32 NVS preferences in flash, so that it survives power-off and reset without needing an SD card.

At power-on or reset the SD card slot is checked for the presence of an SD card with configuration files:

- wifi.txt: textfile containing a single line with Wifi SSID and password separated by a | (pipe). No spaces or quotes.
- players.txt: textfile containing up to 5 lines each with a player name and ipv4-address and port number separated by a | (pipe). No spaces or quotes.
- favs.txt: textfile containing up to 50 lines each with a favourite name and an url separated by a | (pipe). No spaces or quotes.

See the [example_config](https://github.com/dheijl/M5Core2MpdCli/tree/main/example_config) folder for an example of these files.

If present these files are read in and copied to internal ESP32 NVS preferences in flash. At subsequents power-ons this flash config is used (unless an SD card is present with a new config).

After this initialization the WiFi connection is established, and the _status/current file or url/currentsong_ of the active MPD player is shown for a couple of seconds. The screen is turned off immediately to conserve battery power. The wifi connection is kept active for 10 seconds after the last button press (except battery status as it does not need wifi). The green LED is ON while the WiFi connection is established, and OFF when there is currently no WiFi connection. The WiFi connection will be re-established as needed, and dropped after the 10 second time-out.
If the battery is not charging the power is also turned off automatically after the wifi is turned off, as powering up the device, loading the config from flash and establishing the wifi connection hardly takes a second.

My favourites are all internet radio stations in my use case. I haven't tried anything else.

## GUI

The GUI is extremely basic, with this display one could do much better. But I hate GUI design, and it gives me the information I need. Feel free to make pull resuests if GUI is your thing...
