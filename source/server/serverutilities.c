#include "serverutilities.h"

void handle_request(const int socket_fd)
{
   // Retrieve text string and key string and send back the processed text
    for(;;) {
        // Read a pair of characters from the stream
        char pair[2];
        switch (fetch_pair(socket_fd, pair)) {
            // Error
            case -1:
                err(EXIT_FAILURE, "fetch_pair");
            // EOF
            case 0:
                close(socket_fd);
                return;
            default:
                break;
        }

        // Process the character using the key, using -DENC compilation flag to set program behavior
        #ifdef ENC  // Encode character
        char c = encode(pair);
        #else       // Decode character
        char c = decode(pair);
        #endif

        if(write(socket_fd, &c, 1) == -1)
            err(EXIT_FAILURE, "write");
    }
}

int fetch_pair(const int fd, char pair_buf[])
{
    // Attempt to read a pair of bytes from the stream
    char* buf_ptr = pair_buf;
    for (int i = 0; i < 2; i++) {
        const int num_read = read(fd, buf_ptr, 1);
        if(num_read == -1) {
            if (errno == EINTR)     // Interrupt, restart read (i will increment when loop restarts)
                --i;
            else                    // Error in read, return
                return -1;
        }
        else if (num_read == 0) {   // EOF, return
            return 0;
        }
        else {  // Advance the pointer to store the second item
            ++buf_ptr;
        }
    }

    return 2;
}

void reap(int sig)
{
    const int saved_err = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;
    errno = saved_err;
}