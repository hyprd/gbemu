# gbemu
GameBoy emulator written in C++

CPU works, although the timer is inaccurate (for some reason I will find out at some point).

Passes every Blargg instruction test aside from the timer-specific test. Since there's no PPU support currently output for these tests in written to the serial port 0xFF02.

Need to supply your own SDL2 lib and add the DLL to the PATH. Pass your ROM as an argument in the project settings.
