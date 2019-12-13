#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "src/arc4.h"
#include "src/sha256.h"
#include "src/serpent.h"
#include "src/rijndael.h"
#include "src/blowfish.h"
#include "src/threefish-512.h"

#include "src/myfunctions.h"

#define ARC4             0
#define AES              1
#define SERPENT          2
#define BLOWFISH         3
#define THREEFISH        4

#define ENCRYPT       0x00
#define DECRYPT       0xDE

#define DATA_SIZE     4096

#define NAME_CIPHER_ERROR(name) \
        printf("[!] Name cipher \"%s\" incorrect!\n", name)

#define MEMORY_ERROR \
        printf("[!] Cannot allocate memory!\n")

const uint8_t * PROGRAMM_NAME    = "PlexusTCL Console Crypter 4.27 12DEC19 [RU]";

const uint8_t * OPERATION_NAME[] = {"Encrypt", "Decrypt", "Stream cipher"};
const uint8_t * ALGORITM_NAME[]  = {"ARC4", "AES-CFB", "SERPENT-CFB",
                                    "BLOWFISH-CFB", "THREEFISH-512-CFB"};

uint8_t       * rijndael_ctx  = NULL;
SERPENT_CTX   * serpent_ctx   = NULL;
BLOWFISH_CTX  * blowfish_ctx  = NULL;
THREEFISH_CTX * threefish_ctx = NULL;

typedef struct {
  uint8_t data[DATA_SIZE];
  uint8_t post[DATA_SIZE];
} MEMORY_CTX;

void sha256sum(SHA256_CTX * ctx, uint8_t * data, int len) {
  int count = (len * 2) - 1;

  sha256_init(ctx);

  for (int i = 0; i < count; i++) {  /* MAX = 511 */
    sha256_update(ctx, data, len);
  }

  sha256_final(ctx);
}

void hash_to_key(uint8_t * input, uint8_t * output, int len) {
  int i, j;
  uint8_t temp;

  for (i = j = 0; i < len; i++) {
    temp = input[j];
    output[i] = temp;
    j = (j == 31 ? 0 : ++j);
  }
}

void cent(short int * number) {
  if (*number > 100) {
    *number = 100;
  }
}

int operation_variant(int cipher, int operation) {
  return (cipher ? (operation ? 1 : 0) : 2);
}

int filecrypt(uint8_t * finput, uint8_t * foutput, uint8_t * vector, int block_size, int cipher, int operation) {

  FILE * fi = fopen(finput, "rb");

  if (fi == NULL) {
    return -1;
  }

  FILE * fo = fopen(foutput, "wb");

  if (fo == NULL) {
    fclose(fi);
    return -2;
  }

  fseek(fi, 0, SEEK_END);
  long int fsize = ftell(fi);
  fseek(fi, 0, SEEK_SET);

  if ((fsize == -1L) || (fsize == 0)) {
    fclose(fi);
    fclose(fo);
    return -3;
  }

  int memory_len = sizeof(MEMORY_CTX);
  MEMORY_CTX * memory = (MEMORY_CTX *) calloc(1, memory_len);

  if (memory == NULL) {
    fclose(fi);
    fclose(fo);
    return -4;
  }

  float div = (float)fsize / 100.0;

  short int nblock;

  long  int position = 0;
  short int realread = 0;

  short int real_percent = 0;
  short int past_percent = 0;

  uint8_t progress_bar[26] = {0};
  memset(progress_bar, '-', 25);

  while (position < fsize) {
    if (cipher != ARC4 && position == 0) {
      switch (operation) {
        case ENCRYPT: switch (cipher) {
                        case AES:       rijndael_encrypt(rijndael_ctx, vector, memory->post);
                                        break;
                        case SERPENT:   serpent_encrypt(serpent_ctx, (unsigned long *)vector, (unsigned long *)memory->post);
                                        break;
                        case BLOWFISH:  memmove(memory->post, vector, block_size);
                                        blowfish_encrypt(blowfish_ctx, (unsigned long *)memory->post, (unsigned long *)(memory->post + 4));
                                        break;
                        case THREEFISH: threefish_encrypt(threefish_ctx, (U64*)(vector), (U64*)memory->post);
                                        break;
                      }
                      memmove(vector, memory->post, block_size);
                      if (fwrite(vector, 1, block_size, fo) != block_size) {
                        fclose(fi);
                        fclose(fo);

                        free(memory);
                        memory = NULL;

                        return -5;
                      }
                      else
                        fflush(fo);
                      break;
        case DECRYPT: if (fread(vector, 1, block_size, fi) != block_size) {
                        fclose(fi);
                        fclose(fo);

                        free(memory);
                        memory = NULL;

                        return -6;
                      }
                      position += block_size;
                      break;
      }
    }

    realread = (short int)fread(memory->data, 1, DATA_SIZE, fi);

    if (cipher == ARC4) {
      arc4(memory->data, memory->post, realread);
    }
    else {
      for (nblock = 0; nblock < realread; nblock += block_size) {
        switch (cipher) {
          case AES:       rijndael_encrypt(rijndael_ctx, vector, memory->post + nblock);
                          break;
          case SERPENT:   serpent_encrypt(serpent_ctx, (unsigned long *)vector, (unsigned long *)(memory->post + nblock));
                          break;
          case BLOWFISH:  blowfish_encrypt(blowfish_ctx, (unsigned long *)vector, (unsigned long *)(vector + 4));
                          memmove(memory->post + nblock, vector, block_size);
                          break;
          case THREEFISH: threefish_encrypt(threefish_ctx, (U64*)(vector), (U64*)(memory->post + nblock));
                          break;
        }

        strxor(memory->post + nblock, memory->data + nblock, block_size);
        memmove(vector, (operation ? memory->data : memory->post) + nblock, block_size);
      }
    }

    if (fwrite(memory->post, 1, realread, fo) != realread) {
      fclose(fi);
      fclose(fo);

      free(memory);
      memory = NULL;

      return -5;
    }
    else
      fflush(fo);

    position += realread;
    real_percent = (short int)((float)position / div + 0.1);

    cent(&real_percent);

    if (real_percent > past_percent) {

      if ((real_percent % 4) == 0) {
        memset(progress_bar, '#', (real_percent / 4));
      }

      printf("\r >  %s ]%s[ %3d %%", OPERATION_NAME[operation_variant(cipher, operation)], progress_bar, real_percent);

      past_percent = real_percent;
    }
  }

  putc('\n', stdout);

  fclose(fi);
  fclose(fo);

  memset(memory, 0x00, memory_len);
  free(memory);
  memory = NULL;

  return 0;
}

short int vector_init(uint8_t * data, short int size) {
  short int i;

  for (i = 0; i < size; i++)
    data[i] = (uint8_t)genrand(0, 255);

  size = size - 2;

  for (i = 0; i < size; i++) {
    if (data[i] == data[i + 1] && data[i + 1] == data[i + 2])
      break;
  }

  return i;
}

int main (int argc, uint8_t * argv[]) {
  int cipher_number,
      operation,
      key_len,
      block_size = 0;

  if (argc == 2) {
    if (strcmp(argv[1], "--help") == 0) {
      printf("%s\n", PROGRAMM_NAME);
      printf("This is software for encrypt/decrypt files.\n\n");
      printf("Algoritms:   -a/--arc4, -r/--aes, -s/--serpent, -b/--blowfish, -t/--threefish.\n");
      printf("Operation:   -e/--encrypt, -d/--decrypt.\n");
      printf("Lengths key: --128, --192, --256.\n\n");
      printf("Enter: [programm name] [--algoritm] [--operation]"
      	     " [--key length] [input filename] [output filename] [key filename or string key]\n");
      return 0;
    }
    else {
      printf("[!] Incorrect parameter \"%s\"! Only \"--help\".\n", argv[1]);
      return -1;
    }
  }
  else
  if (argc == 5) { /* STREAM CIPHER */
    /* crypter --arc4 input output key */
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
    /* crypter --blowfish --encrypt/decrypt input output key */
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
    /* crypter --aes --encrypt/decrypt --256 input output key */
    /*       0     1                 2     3     4      5   6*/
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
    if (strcmp(argv[3], "--128") == 0) {
      if (cipher_number == AES) {
        Nk = 4;
        Nr = 10;
      }
      key_len = 128;
    }
    else
    if (strcmp(argv[3], "--192") == 0) {
      if (cipher_number == AES) {
        Nk = 6;
        Nr = 12;
      }
      key_len = 192;
    }
    else
    if (strcmp(argv[3], "--256") == 0) {
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

  key_len = (key_len / 8);

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

  if (strcmp(argv[argc - 3], argv[argc - 2]) == 0) {
    printf("[!] Names input and output files equal!\n");
    return -1;
  }
  else
  if (strcmp(argv[argc - 2], argv[argc - 1]) == 0) {
    printf("[!] Names keyfile and output files equal!\n");
    return -1;
  }
  else
  if (strcmp(argv[argc - 3], argv[argc - 1]) == 0) {
    printf("[!] Names keyfile and input files equal!\n");
    return -1;
  }

  uint8_t * buffer = (uint8_t*)calloc(key_len, 1);

  if (buffer == NULL) {
    MEMORY_ERROR;
    return -1;
  }

  int ctx_len = 0;
  int real_read = (short int)readfromfile(argv[argc - 1], buffer, key_len);

  if (real_read == key_len)
    printf("[#] Crypt key read from file \"%s\"!\n", argv[argc - 1]);
  else
  if (real_read > 0 && real_read < key_len) {
    printf("[!] Data in key file %d byte; necessary %d byte!\n", real_read, key_len);
    memset(buffer, 0x00, key_len);    
    free(buffer);
    buffer = NULL;
    return -1;
  }
  else
  if (real_read == 0 || real_read == -1) {
    real_read = strlen(argv[argc - 1]);
    if (real_read > 7 && real_read < 257) {
      ctx_len = sizeof(SHA256_CTX);
      SHA256_CTX * sha256_ctx = (SHA256_CTX*) calloc(1, ctx_len);

      if (sha256_ctx != NULL) {
        sha256sum(sha256_ctx, argv[argc - 1], real_read);
        hash_to_key(sha256_ctx->hash, buffer, key_len);

        memset(sha256_ctx, 0x00, ctx_len);
        free(sha256_ctx);
        sha256_ctx = NULL;

        printf("[#] Crypt key read from command line!\n");
      }
      else {
        MEMORY_ERROR;
        memset(buffer, 0x00, key_len);
        free(buffer);
        buffer = NULL;
      }
    }
    else {
      printf("[!] Data in string key %d byte; necessary 8..256 byte", real_read);
      memset(buffer, 0x00, key_len);
      free(buffer);
      buffer = NULL;
      return -1;
    }
  }

  hexprint(1, buffer, key_len);

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
      MEMORY_ERROR;
      memset(buffer, 0x00, key_len);
      free(buffer);
      buffer = NULL;

      return -1;
    }
  }

  if (cipher_number != ARC4 && operation == ENCRYPT) {
    srand(time(NULL));

    if (vector_init(vector, block_size) < (block_size - 2)) {
      printf("[!] System time stopped!\n");
      memset(buffer, 0x00, key_len);
      memset(vector, 0x00, block_size);

      free(buffer);
      free(vector);

      buffer = NULL;
      vector = NULL;
      return -1;
    }
  }

  if (cipher_number == ARC4) {
    arc4_init(buffer, key_len);
  }
  else
  if (cipher_number == AES) {
    ctx_len = Nb * (Nr + 1) * 4;
    rijndael_ctx = (uint8_t *) calloc(ctx_len, 1);
    if (rijndael_ctx == NULL) {
      MEMORY_ERROR;

      if (operation == ENCRYPT)
        memset(vector, 0x00, block_size);

      memset(buffer, 0x00, key_len);
      memset(vector, 0x00, block_size);

      free(buffer);
      free(vector);

      buffer = NULL;
      vector = NULL;

      return -1;
    }

    rijndael_init(buffer, rijndael_ctx);
  }
  else
  if (cipher_number == SERPENT) {
    ctx_len = sizeof(SERPENT_CTX);
    serpent_ctx = (SERPENT_CTX *) calloc(1, ctx_len);
    if (serpent_ctx == NULL) {
      MEMORY_ERROR;

      if (operation == ENCRYPT)
        memset(vector, 0x00, block_size);

      memset(buffer, 0x00, key_len);
      memset(vector, 0x00, block_size);

      free(buffer);
      free(vector);

      buffer = NULL;
      vector = NULL;

      return -1;
    }
    serpent_init(serpent_ctx, key_len * 8, buffer);
  }
  else
  if (cipher_number == BLOWFISH) {
    ctx_len = sizeof(BLOWFISH_CTX);
    blowfish_ctx = (BLOWFISH_CTX*)calloc(1, ctx_len);
    if (blowfish_ctx == NULL) {
      MEMORY_ERROR;

      if (operation == ENCRYPT)
        memset(vector, 0x00, block_size);

      memset(buffer, 0x00, key_len);
      memset(vector, 0x00, block_size);

      free(buffer);
      free(vector);

      buffer = NULL;
      vector = NULL;

      return -1;
    }
    blowfish_init(blowfish_ctx, buffer, key_len);
  }
  else
  if (cipher_number == THREEFISH) {
    ctx_len = sizeof(THREEFISH_CTX);
    threefish_ctx = (THREEFISH_CTX *)calloc(1, ctx_len);
    if (threefish_ctx == NULL) {
      MEMORY_ERROR;

      if (operation == ENCRYPT)
        memset(vector, 0x00, block_size);

      memset(buffer, 0x00, key_len);
      memset(vector, 0x00, block_size);

      free(buffer);
      free(vector);

      buffer = NULL;
      vector = NULL;

      return -1;
    }
    threefish_init(threefish_ctx, (uint64_t*)buffer, (uint64_t*)buffer);
  }

  printf("[#] Algoritm %s initialized!\n", ALGORITM_NAME[cipher_number]);

  memset(buffer, 0x00, key_len);
  free(buffer);
  buffer = NULL;

  printf("[#] Operation %s file \"%s\" started!\n", OPERATION_NAME[operation_variant(cipher_number, operation)], argv[argc - 3]);

  int result = filecrypt(argv[argc - 3], argv[argc - 2], vector, block_size, cipher_number, operation);

  switch (result) {
    case  0:    printf("[#] %s file \"%s\" complete!\n", OPERATION_NAME[operation_variant(cipher_number, operation)], argv[argc - 3]);
                break;
    case -1:    printf("[!] File for %s \"%s\" not opened!\n", OPERATION_NAME[operation_variant(cipher_number, operation)], argv[argc - 3]);
                break;
    case -2:    printf("[!] Output file \"%s\" not opened!\n", argv[argc - 2]);
                break;
    case -3:    printf("[!] Size of input file \"%s\" 0 or more 2 Gb!\n", argv[argc - 3]);
                break;
    case -4:    MEMORY_ERROR;
                break;
    case -5:    printf("[!] Error write in file \"%s\" !\n", argv[argc - 2]);
                break;
    case -6:    printf("[!] Error read form file \"%s\" !\n", argv[argc - 3]);
                break;
  }

  switch (cipher_number) {
    case ARC4:      memset(secret_key, 0x00, key_len);
                    break;

    case AES:       memset(rijndael_ctx, 0x00, ctx_len);
                    free(rijndael_ctx);
                    rijndael_ctx = NULL;
                    break;

    case SERPENT:   memset(serpent_ctx, 0x00, ctx_len);
                    free(serpent_ctx);
                    serpent_ctx = NULL;
                    break;

    case BLOWFISH:  memset(blowfish_ctx, 0x00, ctx_len);
                    free(blowfish_ctx);
                    blowfish_ctx = NULL;
                    break;

    case THREEFISH: memset(threefish_ctx, 0x00, ctx_len);
                    free(threefish_ctx);
                    threefish_ctx = NULL;
                    break;

  }

  if (vector != NULL) {
    memset(vector, 0x00, block_size);
    free(vector);
    vector = NULL;
  }
}