/* This file contains functions and constants related to server processes
 * for the one-time pad system
 */

#ifndef OTP_SERVERUTILITIES_H
#define OTP_SERVERUTILITIES_H

#ifndef QUEUE_LIMIT
#define QUEUE_LIMIT 5
#endif

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <stddef.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cryptography.h"

// Receive key/character pairs of text from a client and send back a single character
// Server will either encode or decode the character using the key
// depending on whether server code was compiled using the -DENC flag
void handle_request(int socket_fd);

// Fetches a pair of characters from a socket stream, returns 2 on success, 0 on EOF, otherwise returns -1
int fetch_pair(int fd, char pair_buf[]);

// Signal handler to reap all zombie processes
void reap(int sig);

#endif //OTP_SERVERUTILITIES_H
