TinyWav
======= 

Inspired by [https://github.com/mhroth/tinywav](https://github.com/mhroth/tinywav) and tailored to our use case in Discord. Only supports 16 bit interleaved signed integer.

Build
-----

    clang tinywav.c tinywav_write.c -o tinywav_write

Usage
-----

    ./tinywav_write data/input_logitech.raw 16000 2 input_logitech.wav
