#include "tinywav.h"

#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv)
{
  if (argc != 5) {
    printf("Usage: %s input_file_name sampling_rate num_channels output_file_name\n",  argv[0]);
    return -1;
  }

  FILE* fpRead = fopen(argv[1], "rb");
  if (fpRead == NULL) {
    printf("Error opening input file: %s\n", argv[2]);
    return -1;
  }

  int rate = atoi(argv[2]);
  int duration = 10;
  int channels = atoi(argv[3]);

  unsigned samples = rate * duration / 1000;

  TinyWav tw;
  int rc = tinywav_open_write(&tw, channels, rate, TW_INT16, TW_INTERLEAVED, argv[4]);
  if (rc != 0) {
    printf("Failed to open wave file for writing: %d", rc);
    return -1;
  }

  
  int16_t* buffer = (int16_t*)malloc(samples * channels * 2);

  while (fread(buffer, 2, samples * channels, fpRead) == samples * channels) {
    tinywav_write_f(&tw, buffer, samples);
  }

  tinywav_close_write(&tw);

  free (buffer);

  return 0;
}
