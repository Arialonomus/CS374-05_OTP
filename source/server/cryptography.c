#include "cryptography.h"

char encode(const char pair[])
{
    // Input is finished, return newline
    if (pair[0] == '\n') { return '\n'; }

    // Encode the character based on the key and return
    const int plaintext = pair[CHAR_INDEX] == ' ' ? SPACE_VALUE : pair[CHAR_INDEX] - CHAR_OFFSET;
    const int key = pair[KEY_INDEX] == ' ' ? SPACE_VALUE : pair[KEY_INDEX] - CHAR_OFFSET;
    const int ciphertext = (plaintext + key) % RANGE;
    return ciphertext == SPACE_VALUE ? ' ' : (char) (ciphertext + CHAR_OFFSET);
}

char decode(const char pair[])
{
    // Input is finished, return newline
    if (pair[0] == '\n') { return '\n'; }

    // Decode the character based on the key and return
    const int ciphertext = pair[CHAR_INDEX] == ' ' ? SPACE_VALUE : pair[CHAR_INDEX] - CHAR_OFFSET;
    const int key = pair[KEY_INDEX] == ' ' ? SPACE_VALUE : pair[KEY_INDEX] - CHAR_OFFSET;
    const int plaintext = (ciphertext - key + RANGE) % RANGE;
    return plaintext == SPACE_VALUE ? ' ' : (char) (plaintext + CHAR_OFFSET);
}