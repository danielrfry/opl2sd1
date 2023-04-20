# OPL-compatible Yamaha YMF825 driver

This is an OPL-compatible driver for Yamaha's [YMF825](https://device.yamaha.com/en/lsi/products/sound_generator/) FM synthesizer chip (also known as SD-1). It allows playback of OPL2 or OPL3 music on SD-1 by translating OPL register writes to their SD-1 equivalents.

See this video for a demonstration of OPL PC game and tracker music played back on SD-1: https://youtu.be/BEgAx0jngKQ

This repository consists of:

- [`hardware/`](hardware/README.md) - breadboard wiring diagram and schematic for a USB to SD-1 interface based on Raspberry Pi Pico
- `firmware/` - firmware for the above device

The firmware supports the RetroWave OPL3 serial protocol, allowing it to be used with software such as the [RetroWave VGM player](https://github.com/SudoMaker/RetroWave), [https://www.scummvm.org/](ScummVM) and [https://dosbox-x.com/](DOSBox-X) that supports the RetroWave OPL3 hardware.

## Building the firmware

The firmware is written in C++ using the [Raspberry Pi Pico SDK](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf). After installing, build the firmware as follows:

```
$ cd firmware
$ cmake -B build
$ make -C build
```

## Playing VGM files

It's possible to use the [RetroWave_Player](https://github.com/SudoMaker/RetroWave) tool to play back OPL2/3 VGM files using this device. For example:

```
RetroWave_Player -t tty -d /dev/cu.usbmodem1234561 HITECH3.vgm
```

## More information

- [Notes on the OPL to SD-1 translation](doc/translation_notes.md)
