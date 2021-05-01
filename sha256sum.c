#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "src/sha256.h"
#include "src/myfunctions.h"

int sha256(const int tumbler, SHA256_CTX * ctx, 
           const uint8_t * data, uint8_t * hash, const int data_length) {

  sha256_init(ctx);

  if (tumbler == 1) {
    sha256_update(ctx, data, data_length);
    sha256_final(ctx, hash);
    return 0;
  }

  FILE * f = fopen((char*)data, "rb");

  if (f == NULL) {
    return -1;
  }

  fseek(f, 0, SEEK_END);
  long int filesize = ftell(f);
  fseek(f, 0, SEEK_SET);

  if (filesize == -1L || filesize < 1) {
    fclose(f);
    return -2;
  }

  uint8_t * buffer = (uint8_t*)calloc(1024, 1);

  if (buffer == NULL) {
    fclose(f);
    return -3;
  }

  float div = (float)filesize / 100.0;

  short real_procent = 0;
  short past_procent = 0;

  long int position = 0;
  long int realread = 0;

  uint8_t progress_bar[26] = {0};
  memset(progress_bar, '-', 25);

  while (position < filesize) {
    realread = fread(buffer, 1, 1024, f);

    sha256_update(ctx, buffer, realread);

    position += realread;

    real_procent = (short int)((float)position / div + 0.1);

    if (real_procent > past_procent) {
      if ((real_procent % 4) == 0)
        memset(progress_bar, '#', real_procent / 4);

      printf("\rProgress ]%s[ %3d %%", progress_bar, real_procent);
      fflush(stdout);
      past_procent = real_procent;
    }
  }

  putc('\n', stdout);
  fclose(f);

  sha256_final(ctx, hash);

  free(buffer);
  buffer = NULL;

  return 0;
}

int main (int argc, char * argv[]) {

  if (argc != 4) {
    printf("[#] Incorrect arguments.\n");
    printf("[#] Enter command: %s [-s/t or --string/table] [-f/s or --file/string] [filename or string]\n", argv[0]);
    return 0;
  }

  int str_length = strlen(argv[3]);

  if (str_length < 1) {
    printf("[#] Error: overflow int type for argument \"%d\" = [%d]\n", 3, str_length);
    return 0;
  }

  uint8_t buffer[32];

  int snark, clark, result;
  
  snark = clark = result = 0;

  int ctx_length = sizeof(SHA256_CTX);
  SHA256_CTX * ctx = (SHA256_CTX*) calloc(1, ctx_length);

  if (ctx == NULL) {
    printf("[#] Cannot allocate memory!\n");
    return 0;
  }

  if ((strcmp(argv[1], "-s") == 0) || (strcmp(argv[1], "--string") == 0))
    snark = 0;
  else
  if ((strcmp(argv[1], "-t") == 0) || (strcmp(argv[1], "--table") == 0))
    snark = 1;
  else {
    printf("[#] Argument \"%s\" incorrect!\n", argv[1]);
    free(ctx);
    ctx = NULL;
    return 0;
  }

  if ((strcmp(argv[2], "-f") == 0) || (strcmp(argv[2], "--file") == 0))
    clark = 0;
  else
  if ((strcmp(argv[2], "-s") == 0) || (strcmp(argv[2], "--string") == 0))
    clark = 1;
  else {
    printf("[#] Argument \"%s\" incorrect!\n", argv[2]);
    free(ctx);
    ctx = NULL;
    return 0;
  }

  result = sha256(clark, ctx, (uint8_t*)argv[3], buffer, str_length);

  switch (result) {
    case -1: printf("[#] File \"%s\" not openned!\n", argv[3]);
             break;
    case -2: printf("[#] Size of file \"%s\" more 2 Gb!\n", argv[3]);
             break;
    case -3: printf("[#] Cannot allocate memory!\n");
             break;
  }

  if (result == -1 || result == -2 || result == -3) {
    free(ctx);
    ctx = NULL;
    return 0;
  }

  hexprint(snark, buffer, 32);

  memset(ctx, 0x00, ctx_length);
  memset(buffer, 0x00, 32);

  free(ctx);
  ctx = NULL;

  return 0;
}
