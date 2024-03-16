/* This file contains functions and constants related to server processes
 * for the one-time pad system
 */

#ifndef OTP_SERVER_H
#define OTP_SERVER_H

#ifndef  MAX_PORT_NUMBER
#define MAX_PORT_NUMBER 65535
#endif

#ifndef QUEUE_LIMIT
#define QUEUE_LIMIT 5
#endif

#include <err.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cryptography.h"

// Receive a key and plaintext from a client and send back an encrypted string
void handle_encryption(int socket_fd);

// Fetches a pair of characters from a socket stream, returns 2 on success, 0 on EOF, otherwise returns -1
int fetch_pair(int fd, char pair_buf[])

// Signal handler to reap all zombie processes
void reap(int sig);

#endif //OTP_SERVER_H
