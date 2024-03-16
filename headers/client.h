/* This file contains shared functions and constants related to client
 * processes for a one-time pad system
 */

#ifndef OTP_CLIENT_H
#define OTP_CLIENT_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef CHAR_INDEX
#define CHAR_INDEX 0
#endif

#ifndef KEY_INDEX
#define KEY_INDEX 1
#endif

// Reads the contents of a text file into a buffer, resizing the buffer if necessary
// Returns number of bytes read, or -1 on error
int readfile(FILE* file, const char* filename, char** buffer, size_t* buf_size);

#endif //OTP_CLIENT_H