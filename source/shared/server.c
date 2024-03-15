#include "server.h"

void handle_encryption()
{

}

void reap(int sig)
{
    const int saved_err = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;
    errno = saved_err;
}