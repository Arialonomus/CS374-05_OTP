#include "network.h"

int open_socket(const char* port, const enum connector_t caller_t)
{
    // Retrieve a list of potential addresses for binding/connecting
    struct addrinfo const hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = caller_t == SERVER ? AI_PASSIVE : 0,
        .ai_protocol = 0,
    };
    struct addrinfo* addresses;
    if (getaddrinfo(caller_t == CLIENT ? HOSTNAME : 0, port, &hints, &addresses) != 0) {
        warn("getaddrinfo");
        return -1;
    }

    // Iterate through list of addresses until a successful bind/connect, or all addresses tried
    int socket_fd = 0;
    int optval = 1;
    struct addrinfo* addr_iter = NULL;
    for (addr_iter = addresses; addr_iter != NULL; addr_iter = addr_iter->ai_next) {
        // Attempt to create a socket, continue if unable
        socket_fd = socket(addr_iter->ai_family, addr_iter->ai_socktype, addr_iter->ai_protocol);
        if (socket_fd == -1) continue;
        if (caller_t == SERVER && setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
            warn("setsockopt");
            return -1;
        }

        // Attempt to connect to or bind the socket at this address, depending on if caller is a client or server
        int result = 0;
        switch (caller_t) {
            case CLIENT:
                result = connect(socket_fd, addr_iter->ai_addr, addr_iter->ai_addrlen);
                break;
            case SERVER:
                result = bind(socket_fd, addr_iter->ai_addr, addr_iter->ai_addrlen);
        }
        // Bind/Connect successful, exit loop
        if (result == 0)
            break;
        // Bind/Connect failed, close socket and continue
        else
            close(socket_fd);
    }
    // Failed to bind/connect socket to any address
    if (addr_iter == NULL) {
        const char* message = caller_t == CLIENT ? "all connection attempts failed" : "could not bind socket to any address";
        warnx("%s", message);
        socket_fd = -1;
    }

    // Free memory for the linked list of addresses and return
    freeaddrinfo(addresses);
    return socket_fd;
}
