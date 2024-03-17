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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "network.h"
#include "clientutilities.h"

int main(int argc, char* argv[])
{
    // Check arg count
    if (argc != 4) {
        errx(EXIT_FAILURE, "invalid number of arguments");
    }
    // Validate port number
    const char* port_num_str = argv[3];
    const int port_num = atoi(port_num_str);
    if (port_num > MAX_PORT_NUMBER || port_num < 0) {
        errx(EXIT_FAILURE, "invalid port number");
    }

    // Open the text file
    const char* text_filename = argv[1];
    FILE* text_file = fopen(text_filename, "r");
    if (text_file == NULL)
        err(EXIT_FAILURE, "open: %s", text_filename);

    // Read text file into buffer
    size_t text_buf_size = 256;
    char* text = malloc(text_buf_size);
    if (text == NULL)
        err(EXIT_FAILURE, "malloc: text");
    const int n_textchars = readfile(text_file, text_filename, &text, &text_buf_size);
    if (n_textchars == -1) {
        exit(EXIT_FAILURE);
    }
    fclose(text_file);

    // Open the key file
    const char* key_filename = argv[2];
    FILE* key_file = fopen(key_filename, "r");
    if (key_file == NULL)
        err(EXIT_FAILURE, "open: %s", key_filename);

    // Read key file into buffer
    char* key = malloc(n_textchars);
    if (key == NULL) {
        err(EXIT_FAILURE, "malloc: key");
    }
    if (fgets(key, n_textchars, key_file) == NULL) {
        err(EXIT_FAILURE, "fgets");
    }
    fclose(text_file);

    // Validate key and text are the same length and insert newline
    const size_t n_keychars = strlen(key);
    if (n_keychars < n_textchars - 1)
        errx(EXIT_FAILURE, "key string is too short");
    key[n_keychars] = '\n';

    // Attempt to open a connection to the socket at target port
    const int connection_fd = open_socket(port_num_str, CLIENT);
    if (connection_fd == -1) {
        exit(EXIT_FAILURE);
    }

    // Send text and key to encryption server in character pairs
    for (int i = 0; i < n_textchars; ++i) {
        char pair[2];
        pair[CHAR_INDEX] = text[i];
        pair[KEY_INDEX] = key[i];
        if(write(connection_fd, &pair, 2) == -1)
            err(EXIT_FAILURE, "write");
    }

    // Close write end of the socket
    shutdown(connection_fd, SHUT_WR);

    // Read processed text from data stream
    for(;;) {
        // Read a character from the stream
        char c = 0;
        const int num_read = read(connection_fd, &c, 1);
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
    close(connection_fd);

    // Cleanup & Exit
    free(text);
    free(key);
    return EXIT_SUCCESS;
}
