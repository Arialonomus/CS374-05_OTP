#include "address.h"

void configure_address(struct sockaddr_in *address, const int port_num)
{
    // Clear out the address struct
    memset((char *) address, '\0', sizeof(*address));

    // The address should be network capable
    address->sin_family = AF_INET;

    // Store the port number
    address->sin_port = htons(port_num);

    // Allow a client at any address to connect to this server
    address->sin_addr.s_addr = INADDR_ANY;
}
