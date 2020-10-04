/*
  Plexus Technology Cybernetic Laboratories;
  Console Cryptography Software v4.70;

  Make date:    04 Oktober 2020;
  Modification: None (Original);
  Language:     English;
*/

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include "src/arc4.h"
#include "src/sha256.h"
#include "src/serpent.h"
#include "src/rijndael.h"
#include "src/blowfish.h"
#include "src/threefish-512.h"

#include "src/xtalw.h"
#include "src/clomul.h" /* Const CLOMUL_CONST and description */

#define ENCRYPT   0x00
#define DECRYPT   0xDE

#define BOUNDARY  2048
#define DATA_SIZE 4096

const char * PARAM_READ_BYTE  = "rb";
const char * PARAM_WRITE_BYTE = "wb";
const char * PROGRAMM_NAME    = "PlexusTCL Console Crypter 4.70 04OKT20 [EN]";

enum {
  ARC4      = 0,
  AES       = 1,
  SERPENT   = 2,
  BLOWFISH  = 3,
  THREEFISH = 4
};

const uint32_t INT_SIZE_DATA[] = {
  1024,
  1048576,
  1073741824
};

const char * CHAR_SIZE_DATA[] = {
  "bt",
  "KiB",
  "MiB",
  "GiB"
};

const char * OPERATION_NAME[] = {
  "Encrypt",
  "Decrypt",
  "Stream"
};

const char * ALGORITM_NAME[]  = {
  "ARC4",
  "AES-CFB",
  "SERPENT-CFB",
  "BLOWFISH-CFB",
  "THREEFISH-512-CFB"
};

typedef struct {
  uint8_t input [DATA_SIZE];
  uint8_t output[DATA_SIZE];
} MEMORY_CTX;

ARC4_CTX      * arc4_ctx      = NULL;
uint8_t       * rijndael_ctx  = NULL;
SERPENT_CTX   * serpent_ctx   = NULL;
BLOWFISH_CTX  * blowfish_ctx  = NULL;
THREEFISH_CTX * threefish_ctx = NULL;

void NAME_CIPHER_ERROR(const char * name) {
  printf("[!] Name cipher \"%s\" incorrect!\n", name);
}

void MEMORY_ERROR(void) {
  printf("[!] Cannot allocate memory!\n");
}

int size_check(uint32_t size) {
  int result = 0;

  if (size < INT_SIZE_DATA[0]) {
    result = 0;
  }
  else
  if (size >= INT_SIZE_DATA[0] && size < INT_SIZE_DATA[1]) {
    result = 1;
  }
  else
  if (size >= INT_SIZE_DATA[1] && size < INT_SIZE_DATA[2]) {
    result = 2;
  }
  else
  if (size >= INT_SIZE_DATA[2]) {
    result = 3;
  }

  return result;
}

float sizetofloatprint(const int status, const float size) {
  return (status ? (size / (float)INT_SIZE_DATA[status - 1]) : size);
}

void clear_end_string(int print_count) {
  if ((print_count > 0) && (print_count < 80)) {
    print_count -= 79;
    for (int i = 0; i < print_count; i++)
      putc((int)0x20, stdout); /* PROBEL */
  }
}

void * KDFCLOMUL(SHA256_CTX * sha256_ctx, const uint8_t * password, const size_t password_len,
                     uint8_t * key, const size_t key_len) {

  size_t i, j, k;
  size_t count = 0;
  uint8_t hash[SHA256_BLOCK_SIZE];

/*
  srand(time(0));
  clock_t min = clock();
*/
  for (i = 0; i < password_len; ++i) {
    count += (((size_t)password[i] + (i + 1)) * CLOMUL_CONST);
    count += (((password_len * CLOMUL_CONST) + key_len) << 1);
    count += CLOMUL_CONST;
  }

  count *= CLOMUL_CONST;
/*
  printf("Count = %d\n", count);
*/
  sha256_init(sha256_ctx);

  for (i = k = 0; i < key_len; ++i, ++k) {
    for (j = 0; j < count; ++j) {
      sha256_update(sha256_ctx, password, password_len);
    }

    sha256_final(sha256_ctx, hash);

    if (k == SHA256_BLOCK_SIZE)
      k = 0;

    key[i] = hash[k];
  }
/*
  printf("Execute time: %4.4f seconds\n", ((double)(clock() - min) / (double)CLOCKS_PER_SEC));
*/
  (void *)meminit((void *)hash, 0x00, SHA256_BLOCK_SIZE);
  count = i = j = k = 0;
  return key;
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

  if (fseek(f, 0, SEEK_END) != 0) {
    return (-1);
  }

  long int result = ftell(f);

  if (fseek(f, 0, SEEK_SET) != 0) {
    return (-1);
  }

  return result;
}

int filecrypt(const char * finput, const char * foutput, uint8_t * vector,
              const size_t block_size, const int cipher, const int operation) {

  FILE * fi = fopen(finput, PARAM_READ_BYTE);

  if (fi == NULL) {
    return (-1);
  }

  FILE * fo = fopen(foutput, PARAM_WRITE_BYTE);

  if (fo == NULL) {
    (void)fclose(fi);
    return (-2);
  }

  long int fsize    = size_of_file(fi);
  long int position = 0;

  if ((fsize == (-1)) || (fsize == 0)) {
    (void)fclose(fi);
    (void)fclose(fo);
    return (-3);
  }

  size_t memory_len = sizeof(MEMORY_CTX);
  MEMORY_CTX * memory = (MEMORY_CTX *)calloc(1, memory_len);

  if (memory == NULL) {
    (void)fclose(fi);
    (void)fclose(fo);
    return (-4);
  }

  float div = (float)fsize / 100.0;

  int   real_check  = 0;
  int   fsize_check = size_check(fsize);
  float fsize_float = sizetofloatprint(fsize_check, (float)fsize);

  int print_real;

  size_t nblock;
  size_t realread = 0;

  short real_percent = 0;
  short past_percent = 0;

  uint8_t progress_bar[26] = {0};
  (void *)meminit((void *)progress_bar, '-', 25);

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
                        (void)fclose(fi);
                        (void)fclose(fo);

                        free((void *)memory);
                        memory = NULL;

                        return (-5);
                      }
                      else {
                        fflush(fo);
                      }
                      break;

        case DECRYPT: if (fread((void *)vector, 1, block_size, fi) != block_size) {
                        (void)fclose(fi);
                        (void)fclose(fo);

                        free((void *)memory);
                        memory = NULL;

                        return (-6);
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

        (void *)strxor(memory->output + nblock, memory->input + nblock, block_size);
        memmove(vector, (operation ? memory->input : memory->output) + nblock, block_size);
      }
    }

    if (fwrite((void *)(memory->output), 1, realread, fo) != realread) {
      (void)fclose(fi);
      (void)fclose(fo);

      free((void *)memory);
      memory = NULL;

      return (-5);
    }
    else
      fflush(fo);

    position += (long int)realread;
    real_percent = (short)((float)position / div + 0.1);

    cent(&real_percent);

    if (real_percent > past_percent) {

      if ((real_percent % 4) == 0) {
        (void *)meminit((void *)progress_bar, '#', (real_percent / 4));

        real_check = size_check(position);

        print_real = printf(" >  %s [%s] (%4.2f %s/%4.2f %s) %3d %%",
          OPERATION_NAME[operation_variant(cipher, operation)],
          progress_bar,
          sizetofloatprint(real_check, (float)position),
          CHAR_SIZE_DATA[real_check],
          fsize_float,
          CHAR_SIZE_DATA[fsize_check],
          real_percent);

        clear_end_string(print_real);
        putc('\r', stdout);

        fflush(stdout);
      }

      past_percent = real_percent;
    }
  }

  putc('\n', stdout);

  (void)fclose(fi);
  (void)fclose(fo);

  (void *)meminit((void *)memory, 0x00, memory_len);
  free((void *)memory);
  memory = NULL;

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

  if (argc > 1 && argc < 8) {
    for (int i = 1; i < (argc - 1); ++i) {
      if (__strnlen(argv[i], BOUNDARY) == BOUNDARY) { /* BOUNDARY = 2048 */
        printf("[!] Warning: argument \"%d\" length more \"%d\"!\n", i, BOUNDARY);
        return (-1);
      }
    }
  }

  size_t ctx_len, block_size;
  int    key_len, real_read, cipher_number, operation, result;

  if (argc == 2) {
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
      printf("%s\n", PROGRAMM_NAME);
      printf("This is software for encrypt/decrypt files.\n\n");
      printf("Algoritms:   -a/--arc4, -r/--aes, -s/--serpent, -b/--blowfish, -t/--threefish.\n");
      printf("Operations:  -e/--encrypt, -d/--decrypt.\n");
      printf("Lengths key: -a/--128, -b/--192, -c/--256.\n\n");
      printf("Enter: [%s] [algoritm] [operation]"
      	     " [key length] [input filename] [output filename] [key filename or string key]\n", argv[0]);
      return 0;
    }
    else {
      printf("[!] Incorrect parameter \"%s\"! Only \"-h\" or \"--help\".\n", argv[1]);
      return (-1);
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
      return (-1);
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
      return (-1);
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
      return (-1);
    }
  }
  else {
    printf("[!] Incorrect arguments!\n");
    printf("[!] Enter: %s [algoritm] [parameters] [input] [output] [key]\n", argv[0]);
    return (-1);
  }

  key_len = 0;

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
      return (-1);
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

  if (key_len > 0)
    key_len /= 8;
  else
    return (-1);

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
      return (-1);
    }
  }

  if (strcmp(argv[argc - 3], argv[argc - 2]) == 0) {
    printf("[!] Names input and output files equal!\n");
    return (-1);
  }
  else
  if (strcmp(argv[argc - 2], argv[argc - 1]) == 0) {
    printf("[!] Names keyfile and output files equal!\n");
    return (-1);
  }
  else
  if (strcmp(argv[argc - 3], argv[argc - 1]) == 0) {
    printf("[!] Names keyfile and input files equal!\n");
    return (-1);
  }

  uint8_t * buffer = (uint8_t*)calloc(key_len, 1);

  if (buffer == NULL) {
    MEMORY_ERROR();
    return (-1);
  }

  real_read = readfromfile(argv[argc - 1], buffer, key_len);

  if (real_read == key_len)
    printf("[#] Crypt key read from file \"%s\"!\n", argv[argc - 1]);
  else
  if ((real_read > 0) && (real_read < key_len)) {
    printf("[!] Data in key file %d byte; necessary %d byte!\n", real_read, key_len);

    (void *)meminit((void *)buffer, 0x00, key_len);
    free((void *)buffer);
    buffer = NULL;

    return (-1);
  }
  else
  if ((real_read == 0) || (real_read == (-1))) {
    real_read = (int)__strnlen(argv[argc - 1], 256);

    if ((real_read > 7) && (real_read < 257)) { /* Max password length = 256 byte; min = 8  */
      ctx_len = sizeof(SHA256_CTX);
      SHA256_CTX * sha256_ctx = (SHA256_CTX*) calloc(1, ctx_len);

      if (sha256_ctx != NULL) {
        /* Password -> crypt key; Pseudo PBKDF2 */
        KDFCLOMUL(sha256_ctx, (uint8_t *)argv[argc - 1], real_read, buffer, key_len);
        (void *)meminit((void *)sha256_ctx, 0x00, ctx_len);

        free((void *)sha256_ctx);
        sha256_ctx = NULL;

        printf("[#] Crypt key read from command line!\n");
      }
      else {
        MEMORY_ERROR();

        (void *)meminit((void *)buffer, 0x00, key_len);
        free((void *)buffer);
        buffer = NULL;

        return (-1);
      }
    }
    else {
      printf("[!] Data in string key %d byte; necessary 8..256 byte!\n", real_read);

      (void *)meminit((void *)buffer, 0x00, key_len);
      free((void *)buffer);
      buffer = NULL;

      return (-1);
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
      (void *)meminit((void *)buffer, 0x00, key_len);
      free((void *)buffer);
      buffer = NULL;

      return (-1);
    }
  }

  if ((cipher_number != ARC4) && (operation == ENCRYPT)) {
    srand((unsigned int)time(NULL));

    if (vector_init(vector, block_size) < (block_size - 2)) {
      printf("[!] System time stopped!\n");
      (void *)meminit((void *)buffer, 0x00, key_len);
      (void *)meminit((void *)vector, 0x00, block_size);

      free((void *)buffer);
      free((void *)vector);

      buffer = NULL;
      vector = NULL;

      return (-1);
    }
  }

  if (cipher_number == ARC4) {
    ctx_len = sizeof(ARC4_CTX);
    arc4_ctx = (ARC4_CTX *) calloc(1, ctx_len);

    if (arc4_ctx == NULL) {
      MEMORY_ERROR();

      (void *)meminit((void *)buffer, 0x00, key_len);
      free((void *)buffer);

      buffer = NULL;

      return (-1);
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
        (void *)meminit((void *)vector, 0x00, block_size);

      (void *)meminit((void *)buffer, 0x00, key_len);

      free((void *)buffer);
      free((void *)vector);

      buffer = NULL;
      vector = NULL;

      return (-1);
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
        (void *)meminit((void *)vector, 0x00, block_size);

      (void *)meminit((void *)buffer, 0x00, key_len);

      free((void *)buffer);
      free((void *)vector);

      buffer = NULL;
      vector = NULL;

      return (-1);
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
        (void *)meminit((void *)vector, 0x00, block_size);

      (void *)meminit((void *)buffer, 0x00, key_len);

      free((void *)buffer);
      free((void *)vector);

      buffer = NULL;
      vector = NULL;

      return (-1);
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
        (void *)meminit((void *)vector, 0x00, block_size);

      (void *)meminit((void *)buffer, 0x00, key_len);

      free((void *)buffer);
      free((void *)vector);

      buffer = NULL;
      vector = NULL;

      return (-1);
    }
    threefish_init(threefish_ctx, (uint64_t*)buffer, (uint64_t*)buffer);
  }

  printf("[#] Algoritm %s initialized!\n", ALGORITM_NAME[cipher_number]);

  (void *)meminit((void *)buffer, 0x00, key_len);
  free((void *)buffer);

  buffer = NULL;

  printf("[#] Operation %s file \"%s\" started!\n", OPERATION_NAME[operation_variant(cipher_number, operation)], argv[argc - 3]);

  result = filecrypt(argv[argc - 3], argv[argc - 2], vector, block_size, cipher_number, operation);

  switch (result) {
    case  0:    printf("[#] %s file \"%s\" complete!\n", OPERATION_NAME[operation_variant(cipher_number, operation)], argv[argc - 3]);
                break;
    case (-1):  printf("[!] File for %s \"%s\" not opened!\n", OPERATION_NAME[operation_variant(cipher_number, operation)], argv[argc - 3]);
                break;
    case (-2):  printf("[!] Output file \"%s\" not opened!\n", argv[argc - 2]);
                break;
    case (-3):  printf("[!] Size of input file \"%s\" 0 or more 2 GiB!\n", argv[argc - 3]);
                break;
    case (-4):  MEMORY_ERROR();
                break;
    case (-5):  printf("[!] Error write in file \"%s\" !\n", argv[argc - 2]);
                break;
    case (-6):  printf("[!] Error read form file \"%s\" !\n", argv[argc - 3]);
                break;
  }

  switch (cipher_number) {
    case ARC4:      (void *)meminit((void *)arc4_ctx, 0x00, ctx_len);
                    free((void *)arc4_ctx);
                    arc4_ctx = NULL;
                    break;

    case AES:       (void *)meminit((void *)rijndael_ctx, 0x00, ctx_len);
                    free((void *)rijndael_ctx);
                    rijndael_ctx = NULL;
                    break;

    case SERPENT:   (void *)meminit((void *)serpent_ctx, 0x00, ctx_len);
                    free((void *)serpent_ctx);
                    serpent_ctx = NULL;
                    break;

    case BLOWFISH:  (void *)meminit((void *)blowfish_ctx, 0x00, ctx_len);
                    free((void *)blowfish_ctx);
                    blowfish_ctx = NULL;
                    break;

    case THREEFISH: (void *)meminit((void *)threefish_ctx, 0x00, ctx_len);
                    free((void *)threefish_ctx);
                    threefish_ctx = NULL;
                    break;

  }

  if (vector != NULL) {
    (void *)meminit((void *)vector, 0x00, block_size);
    free((void *)vector);
    vector = NULL;
  }

  return 0;
}
