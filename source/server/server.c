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

#include "network.h"
#include "serverutilities.h"

int main(int argc, char* argv[])
{
    // Check arg count
    if (argc != 2) {
        errx(EXIT_FAILURE, "invalid number of arguments");
    }
    // Validate port number
    const char* port_num_str = argv[1];
    const int port_num = atoi(port_num_str);
    if (port_num > MAX_PORT_NUMBER || port_num < 0) {
        errx(EXIT_FAILURE, "invalid socket number");
    }

    // Attempt to open a socket for listening
    int listener_fd = open_socket(port_num_str, SERVER);
    if (listener_fd == -1) {
        exit(EXIT_FAILURE);
    }

    // Begin listening on the opened socket
    if (listen(listener_fd, QUEUE_LIMIT) == -1) {
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
        // Accept a client connection, obtaining client's address and opening a socket
        struct sockaddr_storage client_address;
        socklen_t addrlen = sizeof(client_address);
        int const client_fd = accept(listener_fd, (struct sockaddr*) &client_address, &addrlen);
        if (client_fd == -1) {
            warnx("error: accept");
            continue;
        }

        // Fork a child process to execute the encryption
        switch (fork()) {
            // Error
            case -1:
                warn("fork");
                close(client_fd); // Close copy of the client socket
                break;

            // Child Process
            case 0:
                // Unnecessary copy of listener socket
                close(listener_fd);

                // Identify client before accepting data
                int identifier = 0;
                if(read(client_fd, &identifier, 1) < 1)
                    err(EXIT_FAILURE, "read: identifier");
                if (identifier != REQUEST_TYPE) {
                    close(client_fd);
                } else {
                    if(write(client_fd, &identifier, 1) < 1)
                        err(EXIT_FAILURE, "write: request acceptance");
                    handle_request(client_fd);
                }
                goto exit; // Exit infinite loop when processing is complete

            // Parent Process
            default:
                close(client_fd); // Unnecessary copy of connected socket
                break;
        }
    }

    // Cleanup & Exit
    exit:
    return EXIT_SUCCESS;
}