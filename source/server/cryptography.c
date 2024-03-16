#include "cryptography.h"

char encrypt(const char pair[])
{
    // Input is finished, return newline
    if (pair[0] == '\n') { return '\n'; }

    // Encode the character based on the key and return
    const int plaintext = pair[CHAR_INDEX] == ' ' ? SPACE_VALUE : pair[CHAR_INDEX] - CHAR_OFFSET;
    const int key = pair[KEY_INDEX] == ' ' ? SPACE_VALUE : pair[KEY_INDEX] - CHAR_OFFSET;
    const int ciphertext = (plaintext + key) % MODULO;
    return ciphertext == SPACE_VALUE ? ' ' : (char) (ciphertext + CHAR_OFFSET);
}
