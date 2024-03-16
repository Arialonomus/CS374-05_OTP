/* Assignment: CS 374 Assignment 05 - OTP
 * Program: enc_server
 * Author: Jacob Barber
 * UID: 934561945
 *
 * This program establishes a connection with an encryption server and sends
 * a plaintext string and a key, receiving an ecrypted ciphertext in return
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
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

#include "address.h"

int main(int argc, char* argv[])
{
  // Check arg count
  if (argc != 4) {
    errx(EXIT_FAILURE, "invalid number of arguments");
  }
  // Validate port number
  const char* port_num_str = argv[4];
  const int port_num = atoi(port_num_str);
  if (port_num > MAX_PORT_NUMBER || port_num < 0 ) {
    errx(EXIT_FAILURE, "invalid port number");
  }
  // Open the plaintext file
  const int plaintext_fd = open(argv[2], O_RDONLY);
  if (plaintext_fd == -1)
    err(EXIT_FAILURE, "open: %s", argv[2]);
  // Open the key file
  const int key_fd = open(argv[3], O_RDONLY);
  if (key_fd == -1)
    err(EXIT_FAILURE, "open: %s", argv[3]);

  // Retrieve a list of potential addresses for binding
  struct addrinfo const hints = {
    .ai_family = AF_UNSPEC,
    .ai_socktype = SOCK_STREAM,
    .ai_flags = 0,
    .ai_protocol = 0,
  };
  struct addrinfo* result;
  if (getaddrinfo("localhost", port_num_str, &hints, &result) != 0) {
    err(EXIT_FAILURE, "getaddrinfo");
  }

  // Attempt to bind to each address until successful
  int socket_fd = 0;
  int optval = 1;
  struct addrinfo* res_iter = NULL;
  for (res_iter = result; res_iter != NULL; res_iter = res_iter->ai_next) {
    // Attempt to open the socket, continue if unable
    socket_fd = socket(res_iter->ai_family, res_iter->ai_socktype, res_iter->ai_protocol);
    if (socket_fd == -1) continue;

    // Connection successful, exit loop
    if (connect(socket_fd, res_iter->ai_addr, res_iter->ai_addrlen) == 0) {
      break;
    }

    // Connection faied, close socket and continue
    else {
      close(socket_fd);
    }
  }
  // Failed to connect to any socket
  if (res_iter == NULL) {
    errx(EXIT_FAILURE, "all connection attempts failed");
  }

  // Free memory for the linked list of addresses
  freeaddrinfo(result);

  // Send plaintext and key to encryption server

  // Fetch encrypted ciphertext from data stream

  // Cleanup & Exit
  return EXIT_SUCCESS;
}