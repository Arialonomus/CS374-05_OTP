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
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "address.h"
#include "client.h"

int main(int argc, char* argv[])
{
    // Check arg count
    if (argc != 4) {
        errx(EXIT_FAILURE, "invalid number of arguments");
    }
    // Validate port number
    const char* port_num_str = argv[4];
    const int port_num = atoi(port_num_str);
    if (port_num > MAX_PORT_NUMBER || port_num < 0) {
        errx(EXIT_FAILURE, "invalid port number");
    }

    // Open the plaintext file
    FILE* plaintext_file = fopen(argv[2], "r");
    if (plaintext_file == NULL)
        err(EXIT_FAILURE, "open: %s", argv[2]);

    // Read plaintext file into buffer
    size_t text_buf_size = 256;
    char* plaintext = malloc(text_buf_size);
    if (plaintext == NULL)
        err(EXIT_FAILURE, "malloc: plaintext");
    const int n_textchars = readfile(plaintext_file, &plaintext, &text_buf_size);
    if (n_textchars == -1) {
        free(plaintext);
        exit(EXIT_FAILURE);
    }
    fclose(plaintext_file);

    // Open the key file
    FILE* key_file = fopen(argv[3], "r");
    if (key_file == NULL)
        err(EXIT_FAILURE, "open: %s", argv[3]);

    // Read key file into buffer
    size_t key_buf_size = n_textchars;
    char* key = malloc(n_textchars);
    if (key == NULL)
        err(EXIT_FAILURE, "malloc: key");
    const int n_keychars = readfile(key_file, &key, &key_buf_size);
    if (n_keychars == -1) {
        free(key);
        exit(EXIT_FAILURE);
    }
    fclose(plaintext_file);
    if (n_keychars != n_textchars)
        errx(EXIT_FAILURE, "plaintext and key strings are not the same length");

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

    // Send plaintext and key to encryption server in character pairs
    for (int i = 0; i < n_textchars; ++i) {
        char pair[2];
        pair[CHAR_INDEX] = plaintext[i];
        pair[KEY_INDEX] = key[i];
        if(write(socket_fd, &pair, 2) == -1)
            err(EXIT_FAILURE, "write");
    }

    // Close write end of the socket
    shutdown(socket_fd, SHUT_WR);

    // Read encrypted ciphertext from data stream
    for(;;) {
        // Read a character from the stream
        char c = 0;
        const int num_read = read(socket_fd, &c, 1);
        if (num_read == -1) {
            if (errno == EINTR)     // Interrupted, retry on next loop
                continue;
            err(EXIT_FAILURE, "read");
        }
        if (num_read == 0)
            break;

        // Write the character to output
        if(fputc(c, stdout) == EOF )
            err(EXIT_FAILURE, "fputc");
    }
    close(socket_fd);

    // Cleanup & Exit
    free(plaintext);
    free(key);
    return EXIT_SUCCESS;
}
