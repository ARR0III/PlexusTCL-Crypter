#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>

#include "src/arc4.h"
#include "src/sha256.h"
#include "src/serpent.h"
#include "src/rijndael.h"
#include "src/blowfish.h"
#include "src/threefish-512.h"

#include "src/myfunctions.h"

#define ARC4         0
#define AES          1
#define SERPENT      2
#define BLOWFISH     3
#define THREEFISH    4

#define ENCRYPT   0x00
#define DECRYPT   0xDE

#define DATA_SIZE 4096

const char * PARAM_READ_BYTE  = "rb";
const char * PARAM_WRITE_BYTE = "wb";

const char * PROGRAMM_NAME    = "PlexusTCL Console Crypter 4.61 06JUL20 [RU]";

const char * OPERATION_NAME[] = {"Encrypt", "Decrypt", "Stream"};
const char * ALGORITM_NAME[]  = {"ARC4", "AES-CFB", "SERPENT-CFB",
                                 "BLOWFISH-CFB", "THREEFISH-512-CFB"};

ARC4_CTX      * arc4_ctx      = NULL;
uint8_t       * rijndael_ctx  = NULL;
SERPENT_CTX   * serpent_ctx   = NULL;
BLOWFISH_CTX  * blowfish_ctx  = NULL;
THREEFISH_CTX * threefish_ctx = NULL;

typedef struct {
  uint8_t input [DATA_SIZE];
  uint8_t output[DATA_SIZE];
} MEMORY_CTX;

void NAME_CIPHER_ERROR(const char * name) {
  printf("[!] Name cipher \"%s\" incorrect!\n", name);
}

void MEMORY_ERROR(void) {
  printf("[!] Cannot allocate memory!\n");
}

void password_to_key(SHA256_CTX * sha256_ctx, const uint8_t * password, const size_t password_len,
                     uint8_t * key, const size_t key_len) {

  size_t i, j, k;
  size_t count = key_len + (password_len * 2) - 1; /* MAX = 196,352 */

  uint8_t hash[SHA256_BLOCK_SIZE];

  sha256_init(sha256_ctx);

  for (i = k = 0; i < key_len; ++i, ++k) {
    for (j = 0; j < count; ++j) {
      sha256_update(sha256_ctx, password, password_len);
      sha256_final(sha256_ctx, hash);
    }

    if (k == SHA256_BLOCK_SIZE)
      k = 0;

    key[i] = hash[k];
  }

  memset((void *)hash, 0x00, SHA256_BLOCK_SIZE);
  count = i = j = k = 0;
}

void cent(short * number) {
  if (*number > 100) {
    *number = 100;
  }
}

int operation_variant(const int cipher, const int operation) {
  return (cipher ? (operation ? 1 : 0) : 2);
}

long int size_of_file(FILE * f) {

  fseek(f, 0, SEEK_END);
  long int result = ftell(f);
  fseek(f, 0, SEEK_SET);

  return result;
}

int filecrypt(const char * finput, const char * foutput, uint8_t * vector,
              const size_t block_size, const int cipher, const int operation) {

  FILE * fi = fopen(finput, PARAM_READ_BYTE);

  if (fi == NULL) {
    return -1;
  }

  FILE * fo = fopen(foutput, PARAM_WRITE_BYTE);

  if (fo == NULL) {
    fclose(fi);
    return -2;
  }

  long int fsize    = size_of_file(fi);
  long int position = 0;

  if ((fsize == -1L) || (fsize == 0)) {
    fclose(fi);
    fclose(fo);
    return -3;
  }

  size_t memory_len = sizeof(MEMORY_CTX);
  MEMORY_CTX * memory = (MEMORY_CTX *)calloc(1, memory_len);

  if (memory == NULL) {
    fclose(fi);
    fclose(fo);
    return -4;
  }

  float div = (float)fsize / 100.0;

  size_t nblock;
  size_t realread = 0;

  short real_percent = 0;
  short past_percent = 0;

  uint8_t progress_bar[26] = {0};
  memset((void *)progress_bar, '-', 25);

  while (position < fsize) {
    if ((cipher != ARC4) && (position == 0)) {
      switch (operation) {
        case ENCRYPT: switch (cipher) {
                        case AES:       rijndael_encrypt(rijndael_ctx, vector, memory->output);
                                        break;
                        case SERPENT:   serpent_encrypt(serpent_ctx, (unsigned long *)vector, (unsigned long *)memory->output);
                                        break;
                        case BLOWFISH:  memmove(memory->output, vector, block_size);
                                        blowfish_encrypt(blowfish_ctx, (unsigned long *)memory->output, (unsigned long *)(memory->output + 4));
                                        break;
                        case THREEFISH: threefish_encrypt(threefish_ctx, (uint64_t*)(vector), (uint64_t*)memory->output);
                                        break;
                      }

                      memmove(vector, memory->output, block_size);

                      if (fwrite((void *)vector, 1, block_size, fo) != block_size) {
                        fclose(fi);
                        fclose(fo);

                        free((void *)memory);

                        return -5;
                      }
                      else {
                        fflush(fo);
                      }
                      break;

        case DECRYPT: if (fread((void *)vector, 1, block_size, fi) != block_size) {
                        fclose(fi);
                        fclose(fo);

                        free((void *)memory);

                        return -6;
                      }
                      position += (long int)block_size;
                      break;
      }
    }

    realread = fread((void *)(memory->input), 1, DATA_SIZE, fi);

    if (cipher == ARC4) {
      arc4(arc4_ctx, memory->input, memory->output, realread);
    }
    else {
      for (nblock = 0; nblock < realread; nblock += block_size) {
        switch (cipher) {
          case AES:       rijndael_encrypt(rijndael_ctx, vector, memory->output + nblock);
                          break;
          case SERPENT:   serpent_encrypt(serpent_ctx, (unsigned long *)vector, (unsigned long *)(memory->output + nblock));
                          break;
          case BLOWFISH:  blowfish_encrypt(blowfish_ctx, (unsigned long *)vector, (unsigned long *)(vector + 4));
                          memmove(memory->output + nblock, vector, block_size);
                          break;
          case THREEFISH: threefish_encrypt(threefish_ctx, (uint64_t*)(vector), (uint64_t*)(memory->output + nblock));
                          break;
        }

        strxor(memory->output + nblock, memory->input + nblock, block_size);
        memmove(vector, (operation ? memory->input : memory->output) + nblock, block_size);
      }
    }

    if (fwrite((void *)(memory->output), 1, realread, fo) != realread) {
      fclose(fi);
      fclose(fo);

      free((void *)memory);

      return -5;
    }
    else
      fflush(fo);

    position += (long int)realread;
    real_percent = (short)((float)position / div + 0.1);

    cent(&real_percent);

    if (real_percent > past_percent) {

      if ((real_percent % 4) == 0) {
        memset((void *)progress_bar, '#', (real_percent / 4));

        printf("\r >  %s ]%s[ %3d %%", OPERATION_NAME[operation_variant(cipher, operation)], progress_bar, real_percent);
        fflush(stdout);
      }

      past_percent = real_percent;
    }
  }

  putc('\n', stdout);

  fclose(fi);
  fclose(fo);

  memset((void *)memory, 0x00, memory_len);
  free((void *)memory);

  return 0;
}

size_t vector_init(uint8_t * data, size_t size) {
  size_t i;

  for (i = 0; i < size; i++)
    data[i] = (uint8_t)genrand(0x00, 0xFF);

  size = size - 2;

  for (i = 0; i < size; i++) {
    if ((data[i] == data[i + 1]) && (data[i + 1] == data[i + 2]))
      break;
  }

  return i;
}

int main (int argc, char * argv[]) {
  size_t ctx_len, block_size;
  int    key_len, real_read, cipher_number, operation, result;

  if (argc == 2) {
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
      printf("%s\n", PROGRAMM_NAME);
      printf("This is software for encrypt/decrypt files.\n\n");
      printf("Algoritms:   -a/--arc4, -r/--aes, -s/--serpent, -b/--blowfish, -t/--threefish.\n");
      printf("Operation:   -e/--encrypt, -d/--decrypt.\n");
      printf("Lengths key: -a/--128, -b/--192, -c/--256.\n\n");
      printf("Enter: [%s] [algoritm] [operation]"
      	     " [key length] [input filename] [output filename] [key filename or string key]\n", argv[0]);
      return 0;
    }
    else {
      printf("[!] Incorrect parameter \"%s\"! Only \"-h\" or \"--help\".\n", argv[1]);
      return -1;
    }
  }
  else
  if (argc == 5) { /* STREAM CIPHER */
    /* program --arc4 input output key */
    /*       0      1     2      3   4 */
    if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--arc4") == 0)
      cipher_number = ARC4;
    else {
      NAME_CIPHER_ERROR(argv[1]);
      return -1;
    }
  }
  else
  if (argc == 6) { /* BLOCK CIPHER */
    /* program --blowfish --encrypt/decrypt input output key */
    /*       0          1                 2     3      4   5 */
    if (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--blowfish") == 0)
      cipher_number = BLOWFISH;
    else
    if (strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--threefish") == 0)
      cipher_number = THREEFISH;
    else {
      NAME_CIPHER_ERROR(argv[1]);
      return -1;
    }
  }
  else
  if (argc == 7) {
    /* program --aes --encrypt/decrypt --256 input output key */
    /*       0     1                 2     3     4      5   6 */
    if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--aes") == 0)
      cipher_number = AES;
    else
    if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--serpent") == 0)
      cipher_number = SERPENT;
    else {
      NAME_CIPHER_ERROR(argv[1]);
      return -1;
    }
  }
  else {
    printf("[!] Incorrect arguments!\n");
    printf("[!] Enter: %s [algoritm] [parameters] [input] [output] [key]\n", argv[0]);
    return -1;
  }

  if (cipher_number == ARC4)
    key_len = 2048;
  else
  if (cipher_number == AES || cipher_number == SERPENT) { // AES or SERPENT
    if (strcmp(argv[3], "-a") == 0 || strcmp(argv[3], "--128") == 0) {
      if (cipher_number == AES) {
        Nk = 4;
        Nr = 10;
      }
      key_len = 128;
    }
    else
    if (strcmp(argv[3], "-b") == 0 || strcmp(argv[3], "--192") == 0) {
      if (cipher_number == AES) {
        Nk = 6;
        Nr = 12;
      }
      key_len = 192;
    }
    else
    if (strcmp(argv[3], "-c") == 0 || strcmp(argv[3], "--256") == 0) {
      if (cipher_number == AES) {
        Nk = 8;
        Nr = 14;
      }
      key_len = 256;
    }
    else {
      printf("[!] Key length \"%s\" incorrect!\n", argv[3]);
      return -1;
    }
  }
  else
  if (cipher_number == BLOWFISH) {
    key_len = 448;
  }
  else
  if (cipher_number == THREEFISH) {
    key_len = 512;
  }

  key_len /= 8;

  /*
    ARC4      = (key_len = 256);
    AES       = (key_len = 16 or 24 or 32;
    SERPENT   = (key_len = 16 or 24 or 32);
    BLOWFISH  = (key_len = 56);
    THREEFISH = (key_len = 64);
  */

  if (cipher_number != ARC4) {
    if (strcmp(argv[2], "-e") == 0 || strcmp(argv[2], "--encrypt") == 0) {
      operation = ENCRYPT;
    }
    else
    if (strcmp(argv[2], "-d") == 0 || strcmp(argv[2], "--decrypt") == 0) {
      operation = DECRYPT;
    }
    else {
      printf("[!] Operation \"%s\" incorrect!\n", argv[2]);
      return -1;
    }
  }

  if (strcmpi(argv[argc - 3], argv[argc - 2]) == 0) {
    printf("[!] Names input and output files equal!\n");
    return -1;
  }
  else
  if (strcmpi(argv[argc - 2], argv[argc - 1]) == 0) {
    printf("[!] Names keyfile and output files equal!\n");
    return -1;
  }
  else
  if (strcmpi(argv[argc - 3], argv[argc - 1]) == 0) {
    printf("[!] Names keyfile and input files equal!\n");
    return -1;
  }

  uint8_t * buffer = (uint8_t*)calloc(key_len, 1);

  if (buffer == NULL) {
    MEMORY_ERROR();
    return -1;
  }

  real_read = readfromfile(argv[argc - 1], buffer, key_len);

  if (real_read == key_len)
    printf("[#] Crypt key read from file \"%s\"!\n", argv[argc - 1]);
  else
  if ((real_read > 0) && (real_read < key_len)) {
    printf("[!] Data in key file %d byte; necessary %d byte!\n", real_read, key_len);
    memset((void *)buffer, 0x00, key_len);
    free((void *)buffer);
    return -1;
  }
  else
  if ((real_read == 0) || (real_read == -1)) {
    real_read = strlen(argv[argc - 1]);
    if ((real_read > 7) && (real_read < 257)) {
      ctx_len = sizeof(SHA256_CTX);
      SHA256_CTX * sha256_ctx = (SHA256_CTX*) calloc(1, ctx_len);

      if (sha256_ctx != NULL) {
        password_to_key(sha256_ctx, (uint8_t *)argv[argc - 1], real_read, buffer, key_len);

        memset((void *)sha256_ctx, 0x00, ctx_len);
        free((void *)sha256_ctx);

        printf("[#] Crypt key read from command line!\n");
      }
      else {
        MEMORY_ERROR();
        memset((void *)buffer, 0x00, key_len);
        free((void *)buffer);
        return -1;
      }
    }
    else {
      printf("[!] Data in string key %d byte; necessary 8..256 byte!\n", real_read);
      memset((void *)buffer, 0x00, key_len);
      free((void *)buffer);
      return -1;
    }
  }

  printf("[#] Key length %d-bits initialized!\n", key_len * 8);

  switch (cipher_number) {
    case AES:       block_size = 16;
                    break;
    case SERPENT:   block_size = 16;
                    break;
    case BLOWFISH:  block_size =  8;
                    break;
    case THREEFISH: block_size = 64;
                    break;
  }

  uint8_t * vector = NULL;

  if (cipher_number != ARC4) {
    vector = (uint8_t*)calloc(block_size, 1);

    if (vector == NULL) {
      MEMORY_ERROR();
      memset((void *)buffer, 0x00, key_len);
      free((void *)buffer);

      return -1;
    }
  }

  if ((cipher_number != ARC4) && (operation == ENCRYPT)) {
    srand((unsigned int)time(NULL));

    if (vector_init(vector, block_size) < (block_size - 2)) {
      printf("[!] System time stopped!\n");
      memset((void *)buffer, 0x00, key_len);
      memset((void *)vector, 0x00, block_size);

      free((void *)buffer);
      free((void *)vector);

      return -1;
    }
  }

  if (cipher_number == ARC4) {
    ctx_len = sizeof(ARC4_CTX);
    arc4_ctx = (ARC4_CTX *) calloc(1, ctx_len);

    if (arc4_ctx == NULL) {
      MEMORY_ERROR();

      memset((void *)buffer, 0x00, key_len);
      free((void *)buffer);

      return -1;
    }

    arc4_init(arc4_ctx, buffer, key_len);
  }
  else
  if (cipher_number == AES) {
    ctx_len = Nb * (Nr + 1) * 4;
    rijndael_ctx = (uint8_t *) calloc(ctx_len, 1);
    if (rijndael_ctx == NULL) {
      MEMORY_ERROR();

      if (operation == ENCRYPT)
        memset((void *)vector, 0x00, block_size);

      memset((void *)buffer, 0x00, key_len);

      free((void *)buffer);
      free((void *)vector);

      return -1;
    }

    rijndael_init(buffer, rijndael_ctx);
  }
  else
  if (cipher_number == SERPENT) {
    ctx_len = sizeof(SERPENT_CTX);
    serpent_ctx = (SERPENT_CTX *) calloc(1, ctx_len);
    if (serpent_ctx == NULL) {
      MEMORY_ERROR();

      if (operation == ENCRYPT)
        memset((void *)vector, 0x00, block_size);

      memset((void *)buffer, 0x00, key_len);

      free((void *)buffer);
      free((void *)vector);

      return -1;
    }
    serpent_init(serpent_ctx, key_len * 8, buffer);
  }
  else
  if (cipher_number == BLOWFISH) {
    ctx_len = sizeof(BLOWFISH_CTX);
    blowfish_ctx = (BLOWFISH_CTX*)calloc(1, ctx_len);
    if (blowfish_ctx == NULL) {
      MEMORY_ERROR();

      if (operation == ENCRYPT)
        memset((void *)vector, 0x00, block_size);

      memset((void *)buffer, 0x00, key_len);

      free((void *)buffer);
      free((void *)vector);

      return -1;
    }
    blowfish_init(blowfish_ctx, buffer, key_len);
  }
  else
  if (cipher_number == THREEFISH) {
    ctx_len = sizeof(THREEFISH_CTX);
    threefish_ctx = (THREEFISH_CTX *)calloc(1, ctx_len);
    if (threefish_ctx == NULL) {
      MEMORY_ERROR();

      if (operation == ENCRYPT)
        memset((void *)vector, 0x00, block_size);

      memset((void *)buffer, 0x00, key_len);

      free((void *)buffer);
      free((void *)vector);

      return -1;
    }
    threefish_init(threefish_ctx, (uint64_t*)buffer, (uint64_t*)buffer);
  }

  printf("[#] Algoritm %s initialized!\n", ALGORITM_NAME[cipher_number]);

  memset((void *)buffer, 0x00, key_len);
  free((void *)buffer);

  printf("[#] Operation %s file \"%s\" started!\n", OPERATION_NAME[operation_variant(cipher_number, operation)], argv[argc - 3]);

  result = filecrypt(argv[argc - 3], argv[argc - 2], vector, block_size, cipher_number, operation);

  switch (result) {
    case  0:    printf("[#] %s file \"%s\" complete!\n", OPERATION_NAME[operation_variant(cipher_number, operation)], argv[argc - 3]);
                break;
    case -1:    printf("[!] File for %s \"%s\" not opened!\n", OPERATION_NAME[operation_variant(cipher_number, operation)], argv[argc - 3]);
                break;
    case -2:    printf("[!] Output file \"%s\" not opened!\n", argv[argc - 2]);
                break;
    case -3:    printf("[!] Size of input file \"%s\" 0 or more 2 Gb!\n", argv[argc - 3]);
                break;
    case -4:    MEMORY_ERROR();
                break;
    case -5:    printf("[!] Error write in file \"%s\" !\n", argv[argc - 2]);
                break;
    case -6:    printf("[!] Error read form file \"%s\" !\n", argv[argc - 3]);
                break;
  }

  switch (cipher_number) {
    case ARC4:      memset((void *)arc4_ctx, 0x00, ctx_len);
                    free((void *)arc4_ctx);
                    break;

    case AES:       memset((void *)rijndael_ctx, 0x00, ctx_len);
                    free((void *)rijndael_ctx);
                    break;

    case SERPENT:   memset((void *)serpent_ctx, 0x00, ctx_len);
                    free((void *)serpent_ctx);
                    break;

    case BLOWFISH:  memset((void *)blowfish_ctx, 0x00, ctx_len);
                    free((void *)blowfish_ctx);
                    break;

    case THREEFISH: memset((void *)threefish_ctx, 0x00, ctx_len);
                    free((void *)threefish_ctx);
                    break;

  }

  if (vector != NULL) {
    memset((void *)vector, 0x00, block_size);
    free((void *)vector);
  }
}
