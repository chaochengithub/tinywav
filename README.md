TinyWav
======= 

Inspired by [https://github.com/mhroth/tinywav](https://github.com/mhroth/tinywav) and tailored to our use case in Discord. Writer only. Only supports 16 bit interleaved signed integer.

Build
-----

    clang tinywav.c tinywav_test.c -o tinywav_test

Usage
-----

    ./tinywav_test data/input_logitech.raw 16000 2 input_logitech.wav
