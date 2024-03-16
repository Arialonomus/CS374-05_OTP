#include "server.h"

#include <stdlib.h>

void handle_encryption()
{

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