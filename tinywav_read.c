#include "tinywav.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 4800

int main(int argc, char** argv)
{
  if (argc != 2) {
    printf("Usage: %s input_file_name\n",  argv[0]);
    return -1;
  }

  TinyWav tw;
  int rc = tinywav_open_read(&tw, argv[1], TW_INTERLEAVED, TW_INT16);
  if (rc != 0) {
    printf("Failed to open wave file for reading: %s\n", strerror(rc));
    return -1;
  }

  uint16_t channels = tw.h.NumChannels;
  printf("Channels: %u\n", channels);
  printf("Sample rate: %u\n", tw.h.SampleRate);

  int16_t* buffer = (int16_t*)malloc(BLOCK_SIZE * channels * sizeof(int16_t));
  rc = tinywav_read_f(&tw, buffer, BLOCK_SIZE);
  printf("Read: %d\n", rc);
  for (int i = 0; i < BLOCK_SIZE; i++) {
    if (buffer[2 * i] != 0) {
      printf("%d, L: %d, R: %d\n", i, buffer[2 * i], buffer[2 * i + 1]);
    }
  }

  tinywav_close_read(&tw);

  free (buffer);

  return 0;
}
