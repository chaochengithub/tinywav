/**
 * Copyright (c) 2015-2017, Martin Roth (mhroth@gmail.com)
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */



#ifndef _TINY_WAV_
#define _TINY_WAV_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// http://soundfile.sapp.org/doc/WaveFormat/
typedef struct TinyWavHeader {
  uint32_t ChunkID;
  uint32_t ChunkSize;
  uint32_t Format;
  uint32_t Subchunk1ID;
  uint32_t Subchunk1Size;
  uint16_t AudioFormat;
  uint16_t NumChannels;
  uint32_t SampleRate;
  uint32_t ByteRate;
  uint16_t BlockAlign;
  uint16_t BitsPerSample;
  uint32_t Subchunk2ID;
  uint32_t Subchunk2Size;
} TinyWavHeader;

typedef enum TinyWavChannelFormat {
  TW_INTERLEAVED, // channel buffer is interleaved e.g. [LRLRLRLR]
} TinyWavChannelFormat;

typedef enum TinyWavSampleFormat {
  TW_INT16 = 2,  // two byte signed integer
} TinyWavSampleFormat;

typedef struct TinyWav {
  FILE *f;
  TinyWavHeader h;
  int16_t numChannels;
  uint32_t totalFramesWritten;
  TinyWavChannelFormat chanFmt;
  TinyWavSampleFormat sampFmt;
} TinyWav;

/**
 * Open a file for writing.
 *
 * @param numChannels  The number of channels to write.
 * @param samplerate   The sample rate of the audio.
 * @param sampFmt      The sample format (e.g. 16-bit integer or 32-bit float).
 * @param chanFmt      The channel format (how the channel data is layed out in memory)
 * @param path         The path of the file to write to. The file will be overwritten.
 *
 * @return  The error code. Zero if no error.
 */
int tinywav_open_write(TinyWav *tw,
    int16_t numChannels, int32_t samplerate,
    TinyWavSampleFormat sampFmt, TinyWavChannelFormat chanFmt,
    const char *path);

/**
 * Write sample data to file.
 *
 * @param tw   The TinyWav structure which has already been prepared.
 * @param f    A pointer to the sample data to write.
 * @param len  The number of frames to write.
 *
 * @return The error code. Zero if no error.
 */
int tinywav_write_f(TinyWav *tw, const void *f, int len);

/** Stop writing to the file. The Tinywav struct is now invalid. */
void tinywav_close_write(TinyWav *tw);

/** Returns true if the Tinywav struct is available to write or write. False otherwise. */
bool tinywav_isOpen(TinyWav *tw);

#ifdef __cplusplus
}
#endif

#endif // _TINY_WAV_
