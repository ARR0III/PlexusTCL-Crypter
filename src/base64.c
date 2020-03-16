const
  uint8_t * base64table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void base64encode(uint8_t * input, uint8_t * output, int size) {
  int iptr = 0;
  int optr = 0;
  
  int length = size / 3;

  for (int i = 0; i < length; i++) {
    output[optr]     = base64table[input[iptr] >> 2];
    output[optr + 1] = base64table[((input[iptr] & 3) << 4) + (input[iptr + 1] >> 4)];
    output[optr + 2] = base64table[((input[iptr + 1] & 15) << 2) + (input[iptr + 2] >> 6)];
    output[optr + 3] = base64table[input[iptr + 2] & 63];
    
    optr += 4;
    iptr += 3;
  }

  length = size % 3;

  switch (length) {
    case 1: output[optr]     = base64table[input[iptr] >> 2];
            output[optr + 1] = base64table[(input[iptr] & 3) << 4];
            output[optr + 2] = '=';
            output[optr + 3] = '=';
            break;
    case 2: output[optr]     = base64table[input[iptr] >> 2];
            output[optr + 1] = base64table[((input[iptr] & 3) << 4) + (input[iptr+1] >> 4)];
            output[optr + 2] = base64table[(input[iptr+1] & 15) << 2];
            output[optr + 3] = '=';
            break;
  }
}

void base64decode(uint8_t * input, uint8_t * output, int size) {
  int iptr = 0;
  int optr = 0;

  uint8_t temp[4] = {0};

  int length = size / 4;

  for (int i = 0; i < length; i++){
    for (int j = 0; j < 4; j++) {
      if ((input[iptr] >= 65) && (input[iptr] <= 90))
        temp[j] = input[iptr] - 'A';
      else
      if ((input[iptr] >= 97) && (input[iptr] <= 122))
        temp[j] = input[iptr] - 'a' + 26;
      else
      if ((input[iptr] >= 48) && (input[iptr] <= 57))
        temp[j] = input[iptr] - '0' + 52;
      else
      if (input[iptr] == 43)
        temp[j] = 62;
      else
      if (input[iptr] == 47)
        temp[j] = 63;
      else
      if (input[iptr] == 61)
        temp[j] = 0xFF;

      iptr++;
    }

    output[optr] = (temp[0] << 2) | (temp[1] >> 4);

    if ((temp[2] != 0xFF) && (temp[3] == 0xFF)) {
      output[optr + 1] = (temp[1] << 4) | (temp[2] >> 2);
      optr++;
    }
    else
    if (temp[2] != 0xFF) {
      output[optr + 1] = (temp[1] << 4) | (temp[2] >> 2);
      output[optr + 2] = (temp[2] << 6) | temp[3];
      optr += 2;
    }
    optr++;
  }
}