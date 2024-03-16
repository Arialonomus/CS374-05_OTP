/* This file contains functions and constants related to networking and
 * sockets for clients and servers of a one-time-pad program
 */

#ifndef OTP_NETWORK_H
#define OTP_NETWORK_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#ifndef MAX_PORT_NUMBER
#define MAX_PORT_NUMBER 65535
#endif

#ifndef HOSTNAME
#define HOSTNAME "localhost"
#endif

#include <err.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


// Defines the type of connector seeking to open a socket
enum connector_t
{
    CLIENT,
    SERVER
};

// Attempts to open a TCP socket and connect or bind  to it,
// depending on the type of the calling connector.
// Returns the socket number on success, or -1 on error
int open_socket(const char* port, const enum connector_t caller_t);

#endif //OTP_NETWORK_H