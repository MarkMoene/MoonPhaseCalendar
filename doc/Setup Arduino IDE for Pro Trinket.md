Setup Arduino IDE for Pro Trinket
===================================
<br>**Install Driver for Pro Trinket, see Using the USB bootloader:**

- Download the Pro Trinket Drivers for Windows and unzip.
- Connect Pro Trinket to USB port, specify location (toplevel directory) of driver.
- (If needed, manually Update Driver in Windows Device Manager.)
  
![](image/device_manager.png)

<br>**In Arduimo 1.6.6 open File > Preferences:**

- Add Board Manager URL for Adafruit boards (to get list, see below):
  https://adafruit.github.io/arduino-board-index/package_adafruit_index.json

![](image/arduino_preferences.png)

<br>**List of additional Boards Manager URLs (click folder icon in above panel):**

![](image/arduino_additional_boards_managers_urls.png)

<br>**Select board and programmer:**

Open board manager, Tools > Board, Board Manager

- Select Contributed.
- Click Adafruit AVR Boards (Installing...).
- Quit and restart Arduino IDE.
- Select Tools > Board > Pro Trinket 3V/12MHz (USB)
- Select Tools > Programmer > USBtinyISP

<br>**Test if program can be uploaded to Pro Trinket:**

- Load blink example, File > Examples > 01.Basics > Blink
- Compile code: Sketch > Verify/Compile (Ctrl+R)
- Activate bootloader on Trinket: press button on Pro Trinket:
- Red LED glows brighter/less bright
- Upload program: Sketch > Upload (Ctrl+U)::
- Red LED blinks 1s on, 1s off

