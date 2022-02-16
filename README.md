# N64 RandNET Keyboard Test ROM

This is a simple test ROM for Nintendo 64 that detects the RandNET Keyboard peripheral on any controller port and shows which keycodes are currently pressed.

![Screenshot of detected keyboard with keys pressed](./screenshot.png?raw=true)

## Run the test ROM

[Download](./keyboardtest.z64?raw=true) or [compile](#build-the-rom) the ROM file and load it as you would any other N64 ROM.

This ROM file has been tested to work on real Nintendo 64 hardware using the [EverDrive-64 by krikzz](http://krikzz.com/) and [64drive by retroactive](http://64drive.retroactive.be/).

If you do not have access to a real RandNET Keyboard, you can simulate one with a normal USB keyboard using a [`usb64` adapter](https://github.com/Ryzee119/usb64).

As of February 2022, there are no emulators that offer support for the RandNET Keyboard peripheral, but there is an [experimental branch of cen64](https://github.com/meeq/cen64/tree/feature/RandNET-keyboard) that can be used for testing:

```sh
cen64 -controller num=1,type=keyboard pifdata.bin keyboardtest.z64
```

## Build the ROM

1. [Install LibDragon](https://github.com/DragonMinded/libdragon) and make sure you export `N64_INST` as the path to your N64 compiler toolchain.
2. Run `make` to produce `keyboardtest.z64`

## License

This project is [Unlicensed public domain software](./LICENSE.md?raw=true) written by [Christopher Bonhage](https://github.com/meeq).

LibDragon is [Unlicensed public domain software](https://github.com/DragonMinded/libdragon/blob/trunk/LICENSE.md?raw=true).

"Nintendo 64" is a registered trademark of Nintendo used for informational purposes without permission.
