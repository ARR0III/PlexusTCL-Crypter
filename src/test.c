#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* universal function ! NOT CHECK IN WORK !*/
void * strxor_un(void * out, const void * in, size_t lenght) {
  if (!out || !in || (out == in)) {
    return out;
  }

        unsigned char * local_out = (unsigned char *)out;
  const unsigned char * local_in  = (const unsigned char *)in;

        uint32_t * local_32_out = (uint32_t *)local_out;
  const uint32_t * local_32_in  = (uint32_t *)local_in;

        uint64_t * local_64_out = (uint64_t *)local_out;
  const uint64_t * local_64_in  = (uint64_t *)local_in;

  size_t register_size = sizeof(size_t);

  if (8 == register_size) {
    if (local_64_out < local_64_in) {
      while (lenght) {

        if (lenght < register_size)
          break;

        *local_64_out ^= *local_64_in;
      
        ++local_64_out;
        ++local_64_in;

        lenght -= register_size;
      }
    }
    else {
            uint64_t * last_64_out = (uint64_t *)(local_out + (lenght - 1));
      const uint64_t * last_64_in  = (uint64_t *)(local_in  + (lenght - 1));
      while (lenght) {

        if (lenght < register_size)
          break;

        *last_64_out ^= *last_64_in;
      
        --last_64_out;
        --last_64_in;

        lenght -= register_size;
      }
    }
  }
  else
  if (4 == register_size) {
    if (local_32_out < local_32_in) {
      while (lenght) {

        if (lenght < register_size)
          break;

        *local_32_out ^= *local_32_in;
      
        ++local_32_out;
        ++local_32_in;

        lenght -= register_size;
      }
    }
    else {
            uint32_t * last_32_out = (uint32_t *)(local_out + (lenght - 1));
      const uint32_t * last_32_in  = (uint32_t *)(local_in  + (lenght - 1));
      while (lenght) {

        if (lenght < register_size)
          break;

        *last_32_out ^= *last_32_in;
      
        --last_32_out;
        --last_32_in;

        lenght -= register_size;
      }
    }
  }

  if (local_out < local_in) {
    while(lenght--) {
      *local_out ^= *local_in;

      ++local_out;
      ++local_in;
    }
  }
  else {
          unsigned char * last_out = local_out + (lenght - 1);
    const unsigned char * last_in  = local_in  + (lenght - 1);
     
     while (lenght--) {
       *last_out ^= *last_in;

       --last_out;
       --last_in;
     }
  }

  return out;
}

int main(void) {
  unsigned char in[]  = "666666666666666666666666666666666666666";
  unsigned char out[] = "666666666666666666666666666666666666667";
  
  size_t len = strlen((char *)out);
  unsigned char * buffer = (unsigned char *)strxor_un(out, in, len);
  
  for (size_t i = 0; i < len; i++)
    printf("%02X", buffer[i]);

  return 0;
}
