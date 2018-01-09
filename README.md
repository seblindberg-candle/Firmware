# Candle Firmware

The firmware is currently under active development and is as of now not very functional. The todo below is far from complete but at least gives a hint at what still needs to be done.

## TODO

- [ ] Finalize the programmer protocol.
- [ ] Extract device drivers to a separate library.
- [ ] Improve hardware abstraction/separation so that porting and testing is easier.
- [ ] Write tests for the programmer.

## Hardware

The target hardware is being developed in [this repo](https://github.com/seblindberg-candle/Board) but some key specs are

- ATXMega8E5 processor running at 8 MHz
- 8x16 pixel array driven by a constant current shift register
- standard 4 Mbit flash memory

## Programming Protocol

The protocol, run over a serial connection, will only include a few commands. Specifically it will support

- device identification, 
- erasing the contents of the flash and
- programming a the flash.

It is as of yet not decided if the programming will be done one page at a time or if it will support arbitrary writing.
