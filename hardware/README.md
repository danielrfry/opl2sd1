# YMF825 USB Interface

In this directory is a schematic and a breadboard wiring diagram for a USB interface to the YMF825 using the following components:

- [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/) microcontroller board
- [YMF825Board](http://uda.la/fm/fm-e.html) breakout board for the YMF825 chip
- 74AHCT125 quad bus buffer
- 10kΩ resistor

The YMF825Board can be purchased from [Switch Science](https://international.switch-science.com/catalog/3399/), though stock appears to be extremely limited.

The Raspberry Pi Pico uses 3.3v logic, while the YMF825Board uses 5v logic by default. It can be modified to accept 3.3v logic levels, but because of the scarcity of these boards, and the difficulty of the modification (due to the small component size), I opted instead to use a 74AHCT125 buffer as a level shifter.

![Breadboard photograph](OPLSD1.jpg)
