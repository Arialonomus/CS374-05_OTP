#include "client.h"

int readfile(FILE* file, char** buffer, size_t* buf_size)
{
    // Process file until newline or EOF is read
    char c = 0;
    size_t chars_read = 0;

    while (c != '\n') {
        // Read a byte from file
        const int byte = fgetc(file);
        if (byte == EOF) {
            if (ferror(file)) {
                warn("fgetc");
                return -1;
            }
            warnx("file does not contain newline");
            return -1;
        }

        // Validate character
        c = (char) byte;
        if (c != ' ' && c != '\n' && (c < 'A' || c > 'Z')) {
            warnx("invalid character read from file");
            return -1;
        }

        // Place the character in the plaintext buffer, resizing if necessary
        if (chars_read == *buf_size) {
            *buf_size *= 2;
            char* tmp_ptr = realloc(buffer, *buf_size);
            if (tmp_ptr == NULL) {
                warn("realloc");
                return -1;
            }
            *buffer = tmp_ptr;
            tmp_ptr = NULL;
        }
        (*buffer)[chars_read++] = c;
    }

    return chars_read;
}
