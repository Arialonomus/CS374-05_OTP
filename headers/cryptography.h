/* This file contains functions related to encrypting and decrypting messages
 * in a one-time pad system
*/

#ifndef OTP_CRYPTOGRAPHY_H
#define OTP_CRYPTOGRAPHY_H

#ifndef CHAR_INDEX
#define CHAR_INDEX 0
#endif

#ifndef KEY_INDEX
#define KEY_INDEX 1
#endif

#define CHAR_MAX 90
#define CHAR_MIN 65
#define CHAR_OFFSET CHAR_MIN
#define RAND_RANGE (CHAR_MAX - CHAR_MIN) + 2
#define SPACE_VALUE RAND_RANGE - 1
#define MODULO RAND_RANGE - 1

// Returns a ciphertext char based on a passed in pair of plaintext char and key char
char encrypt(char pair[]);

#endif //OTP_CRYPTOGRAPHY_H
