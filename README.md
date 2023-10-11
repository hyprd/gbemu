# gbemu
GameBoy emulator written in C++

CPU works, although the timer is inaccurate (for some reason I will find out at some point).

Passes every Blargg instruction test aside from the timer-specific test. Since there's no PPU support currently output for these tests in written to the serial port 0xFF02.
