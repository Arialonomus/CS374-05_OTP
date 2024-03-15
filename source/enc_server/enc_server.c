/* Assignment: CS 374 Assignment 05 - OTP
 * Program: enc_server
 * Author: Jacob Barber
 * UID: 934561945
 *
 * This program listens on a port and encodes textfiles as requested by a
 * client program, enc_client
 */

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "address.h"
#include "cryptography.h"

int main(int argc, char* argv[])
{
  // Check arg count
  if (argc != 2) {
    errx(EXIT_FAILURE, "invalid number of arguments");
  }
  // Validate port number
  const char* port_num_str = argv[2];
  const int port_num = atoi(port_num_str);
  if (port_num > MAX_PORT_NUMBER || port_num < 0 ) {
    errx(EXIT_FAILURE, "invalid socket number");
  }

  // Retrieve a list of potential addresses for binding
  struct addrinfo const hints = {
    .ai_family = AF_UNSPEC,
    .ai_socktype = SOCK_STREAM,
    .ai_flags = AI_PASSIVE,
    .ai_protocol = 0,
  };
  struct addrinfo* result;
  if (getaddrinfo(0, port_num_str, &hints, &result) != 0) {
    err(EXIT_FAILURE, "getaddrinfo");
  }

  // Attempt to bind to each address until successful
  int socket_fd = 0;
  int optval = 1;
  struct addrinfo* r_ptr = NULL;
  for (r_ptr = result; r_ptr != NULL; r_ptr = r_ptr->ai_next) {
    // Attempt to open the socket, continue if unable
    socket_fd = socket(r_ptr->ai_family, r_ptr->ai_socktype, r_ptr->ai_protocol);
    if (socket_fd == -1) continue;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
      err(EXIT_FAILURE, "setsockopt");
    }

    // Bind successful, exit loop
    if (bind(socket_fd, r_ptr->ai_addr, r_ptr->ai_addrlen) == 0) {
      break;
    }

    // Bind faied, close socket and continue
    else {
      close(socket_fd);
    }
  }
  // Failed to bind to any socket
  if (r_ptr == NULL) {
    errx(EXIT_FAILURE, "could not bind socket to any address");
  }

  // Free memory for the linked list of addresses
  freeaddrinfo(result);

  return EXIT_SUCCESS;
}