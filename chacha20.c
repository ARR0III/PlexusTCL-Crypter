/*
  Test vectors check = True;

  Test vectors information:
    Organization:                      Internet Engineering Task Force, IETF
    Network Working Group                                    J. Strombergson
    Internet-Draft                                        Secworks Sweden AB
    Intended status: Informational                         December 31, 2013

  URL:
    https://tools.ietf.org/html/draft-strombergson-chacha-test-vectors-01
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 64

#include "src/chacha20.h"
#include "src/myfunctions.h"

int main (void) {
  uint8_t key    [32] = {0x00};
  uint8_t iv     [ 8] = {0x00};
  uint8_t input  [BLOCK_SIZE] = {0x00};
  uint8_t output [BLOCK_SIZE] = {0x00};

  uint32_t key_len = sizeof(chacha20_ctx);

  chacha20_ctx * SUPER_KEY = (chacha20_ctx *)calloc(1, key_len);

  if (SUPER_KEY == NULL) {
    printf("[#] Cannot allocate memory!\n");
    return -1;
  }

  chacha20_keysetup(SUPER_KEY, key, 32 * 8);
  chacha20_ivsetup(SUPER_KEY, iv);
  chacha20_crypt(SUPER_KEY, input, output, BLOCK_SIZE);

  printf("ENCRYPT_DATA:\n");
  printhex(1, output, BLOCK_SIZE);

  memset(SUPER_KEY, 0x00, key_len);

  chacha20_keysetup(SUPER_KEY, key, 32 * 8);
  chacha20_ivsetup(SUPER_KEY, iv);
  chacha20_crypt(SUPER_KEY, output, input, BLOCK_SIZE);
  
  printf("DECRYPT_DATA:\n");
  printhex(1, input, BLOCK_SIZE);

  memset(SUPER_KEY, 0x00, key_len);
  free(SUPER_KEY);
  SUPER_KEY = NULL;
  
  return 0;
}