# PlexusTCL Crypter

PlexusTCL Crypter is software designed for cryptographic protection of information by encrypting files up to 2 GB in size. The file can be encrypted with one of six user-selectable encryption algorithms. Algorithms are used: ARC4, Rijndael-128, Serpent, Blowfish, Twofish, Threefish.

All implementations of cryptographic algorithms are verified using test vectors published by their developers, except for the ARC4, which corresponds to an example published anonymously in 1994.

All block bins implemented in the program work in the CFB mode, since the author of the program is too lazy to implement the addition of the last incomplete block when using the CBC mode.
