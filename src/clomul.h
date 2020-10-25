
#define CLOMUL_CONST 0x01U

/*
  ------------------------------------------------------------------------------
  ---EN---
  The CLOMUL_CONST constant denotes the so-called "clock multiplier", used in
  the KDFCLOMUL function as one of the operands when forming the number of
  hashing iterations.

  If we describe the constant in simple words, then the greater its value, the
  it takes longer to generate a key from a password, since key generation
  requires much more processor cycles. It is required to generate the
  encryption key from the password, since it affects the number of iterations
  of the hash sum calculation cycle password. The loop itself uses the
  SHA-2-256 hashing algorithm. Default "clock multiplier" is 1, but increases
  to 12, 16, 38 or 67 forces any hardware to generate a key very slowly, which
  makes a brute-force attack even on a short 8-character password is impossible
  for acceptable time! Handle this constant with care, as it is wrong the
  chosen value may cause you to wait for the end generating a key from a
  relatively short password, minutes or even hours.

  If the "clock multiplier" is too large, then the variable the number of hashing
  iterations may overflow and will store negative value, which means tens of
  minutes of waiting! Choose value based on the total power of your opponent's
  computer park, so how the defense is built only taking into account the
  possible attacker. For 2020 year, the value can be made 3, this will be
  enough for another 10-15 years. Maximum the same value is 80.112. Never
  exceed it, otherwise you will wait years!

  A "clock multiplier" value of 1 ensures key generation encryption from the
  password "password" on an Intel Core I3-3217U processor with a clock
  frequency of 1.8 GHz in about 0.02 seconds, which already turns a complete
  brute force all 8 character passwords into a hell of a nightmare, as to
  brute force all passwords consisting only of lowercase Latin letters, you
  will need to sort out 208,827,064,576 password options. Since the processor
  is able to iterate over about 50 passwords per second, then a full search of
  all passwords will take on the above processor is about 132 years old!

  As the test on the above processor showed, with the "clock multiplier"
  equal to 64, generate a 256-bit encryption key for the Rijndael algorithm,
  when using the password "password", it lasts a whole 47 seconds! Since people
  are not like to wait a long time, the default has been reduced for the sake
  of unloading user processor and comfortable work with the program!

  It is important to note that the longer the password, the longer it takes to
  generate the key, so how the length of the password affects the number of
  iterations of the hash sum calculation loop password, acting as a pseudo
  "clock multiplier". But also not only the length of the password affects the
  number of iterations, but also the length of the generated key, as well as
  each password byte!

  Programs with different "clock multiplier" are INCOMPATIBLE!
  ---EN---
  ------------------------------------------------------------------------------
*/
