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

#include "network.h"
#include "client.h"

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

    // Open the plaintext file
    const char* plaintext_filename = argv[1];
    FILE* plaintext_file = fopen(plaintext_filename, "r");
    if (plaintext_file == NULL)
        err(EXIT_FAILURE, "open: %s", plaintext_filename);

    // Read plaintext file into buffer
    size_t text_buf_size = 256;
    char* plaintext = malloc(text_buf_size);
    if (plaintext == NULL)
        err(EXIT_FAILURE, "malloc: plaintext");
    const int n_textchars = readfile(plaintext_file, plaintext_filename, &plaintext, &text_buf_size);
    if (n_textchars == -1) {
        exit(EXIT_FAILURE);
    }
    fclose(plaintext_file);

    // Open the key file
    const char* key_filename = argv[2];
    FILE* key_file = fopen(key_filename, "r");
    if (key_file == NULL)
        err(EXIT_FAILURE, "open: %s", key_filename);

    // Read key file into buffer
    size_t key_buf_size = n_textchars;
    char* key = malloc(n_textchars);
    if (key == NULL)
        err(EXIT_FAILURE, "malloc: key");
    const int n_keychars = readfile(key_file, key_filename, &key, &key_buf_size);
    if (n_keychars == -1) {
        exit(EXIT_FAILURE);
    }
    fclose(plaintext_file);

    // Validate key and text are the same length
    if (n_keychars != n_textchars)
        errx(EXIT_FAILURE, "plaintext and key strings are not the same length");

    // Attempt to open a connection to the socket at target port
    const int connection_fd = open_socket(port_num_str, CLIENT);
    if (connection_fd == -1) {
        exit(EXIT_FAILURE);
    }

    // Send plaintext and key to encryption server in character pairs
    for (int i = 0; i < n_textchars; ++i) {
        char pair[2];
        pair[CHAR_INDEX] = plaintext[i];
        pair[KEY_INDEX] = key[i];
        if(write(connection_fd, &pair, 2) == -1)
            err(EXIT_FAILURE, "write");
    }

    // Close write end of the socket
    shutdown(connection_fd, SHUT_WR);

    // Read encrypted ciphertext from data stream
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
    free(plaintext);
    free(key);
    return EXIT_SUCCESS;
}
