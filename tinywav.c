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



#include <assert.h>
#include <string.h>
#if _WIN32
#include <winsock2.h>
#include <malloc.h>
#else
#include <alloca.h>
#include <netinet/in.h>
#endif
#include "tinywav.h"

int tinywav_open_write(TinyWav *tw,
    int16_t numChannels, int32_t samplerate,
    TinyWavSampleFormat sampFmt, TinyWavChannelFormat chanFmt,
    const char *path) {
#if _WIN32
  errno_t err = fopen_s(&tw->f, path, "w");
  if (err != 0) {
    return err;
  }
#else
  tw->f = fopen(path, "w");
#endif
  if (!tw->f) {
    return -1;
  }
  tw->numChannels = numChannels;
  tw->totalFramesWritten = 0;
  tw->sampFmt = sampFmt;
  tw->chanFmt = chanFmt;

  // prepare WAV header
  TinyWavHeader h;
  h.ChunkID = htonl(0x52494646); // "RIFF"
  h.ChunkSize = 0; // fill this in on file-close
  h.Format = htonl(0x57415645); // "WAVE"
  h.Subchunk1ID = htonl(0x666d7420); // "fmt "
  h.Subchunk1Size = 16; // PCM
  h.AudioFormat = (uint16_t)(tw->sampFmt-1); // 1 PCM, 3 IEEE float
  h.NumChannels = numChannels;
  h.SampleRate = samplerate;
  h.ByteRate = samplerate * numChannels * tw->sampFmt;
  h.BlockAlign = (uint16_t)(numChannels * tw->sampFmt);
  h.BitsPerSample = (uint16_t)(8*tw->sampFmt);
  h.Subchunk2ID = htonl(0x64617461); // "data"
  h.Subchunk2Size = 0; // fill this in on file-close

  // write WAV header
  return fwrite(&h, sizeof(TinyWavHeader), 1, tw->f) != 1;
}

size_t tinywav_write_f(TinyWav *tw, const void *f, int len) {
  tw->totalFramesWritten += len;
  return fwrite(f, sizeof(int16_t), tw->numChannels*len, tw->f);
}

void tinywav_close_write(TinyWav *tw) {
  uint32_t data_len = tw->totalFramesWritten * tw->numChannels * tw->sampFmt;

  // set length of data
  fseek(tw->f, 4, SEEK_SET);
  uint32_t chunkSize_len = 36 + data_len;
  fwrite(&chunkSize_len, sizeof(uint32_t), 1, tw->f);

  fseek(tw->f, 40, SEEK_SET);
  fwrite(&data_len, sizeof(uint32_t), 1, tw->f);

  fclose(tw->f);
  tw->f = NULL;
}

bool tinywav_isOpen(TinyWav *tw) {
  return (tw->f != NULL);
}
