/* Assignment: CS 374 Assignment 05 - OTP
 * Program: enc_server / dec_server
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
#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "address.h"
#include "serverutilities.h"

int main(int argc, char *argv[])
{
    // Check arg count
    if (argc != 2) {
        errx(EXIT_FAILURE, "invalid number of arguments");
    }
    // Validate port number
    const char *port_num_str = argv[1];
    const int port_num = atoi(port_num_str);
    if (port_num > MAX_PORT_NUMBER || port_num < 0) {
        errx(EXIT_FAILURE, "invalid socket number");
    }

    // Retrieve a list of potential addresses for binding
    struct addrinfo const hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE,
        .ai_protocol = 0,
    };
    struct addrinfo *result;
    if (getaddrinfo(0, port_num_str, &hints, &result) != 0) {
        err(EXIT_FAILURE, "getaddrinfo");
    }

    // Attempt to bind to each address until successful
    int server_socket = 0;
    int optval = 1;
    struct addrinfo *res_iter = NULL;
    for (res_iter = result; res_iter != NULL; res_iter = res_iter->ai_next) {
        // Attempt to open the socket, continue if unable
        server_socket = socket(res_iter->ai_family, res_iter->ai_socktype, res_iter->ai_protocol);
        if (server_socket == -1) continue;
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
            err(EXIT_FAILURE, "setsockopt");
        }

        // Bind successful, exit loop
        if (bind(server_socket, res_iter->ai_addr, res_iter->ai_addrlen) == 0) {
            break;
        }

        // Bind faied, close socket and continue
        else {
            close(server_socket);
        }
    }
    // Failed to bind to any socket
    if (res_iter == NULL) {
        errx(EXIT_FAILURE, "could not bind socket to any address");
    }

    // Free memory for the linked list of addresses
    freeaddrinfo(result);

    // Begin listening on the opened socket
    if (listen(server_socket, QUEUE_LIMIT) == -1) {
        err(EXIT_FAILURE, "listen");
    }

    // Set up signal handling for zombie processes
    struct sigaction disposition;
    sigemptyset(&disposition.sa_mask);
    disposition.sa_flags = SA_RESTART;
    disposition.sa_handler = reap;
    if (sigaction(SIGCHLD, &disposition, NULL) == -1) {
        err(EXIT_FAILURE, "sigaction");
    }

    // Handle client requests
    for (;;) {
        // Accept a client connection, obtaining client's address
        struct sockaddr_storage client_address;
        socklen_t addrlen = sizeof(client_address);
        int const client_socket = accept(server_socket, (struct sockaddr *) &client_address, &addrlen);
        if (client_socket == -1) {
            warnx("error: accept");
            continue;
        }

        // Fork a child process to execute the encryption
        switch (fork()) {
            // Error
            case -1:
                warn("fork");
                close(client_socket); // Unnecessary copy of listening socket
                break;

            // Child Process
            case 0:
                close(server_socket);
                handle_encryption(client_socket);
                goto exit; // Exit infinite loop when processing is complete

            // Parent Process
            default:
                close(client_socket); // Unnecessary copy of connected socket
                break;
        }
    }

    // Cleanup & Exit
    exit:
    return EXIT_SUCCESS;
}
