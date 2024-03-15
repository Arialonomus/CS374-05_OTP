/* This file contains functions related to address handling for network sockets
 */

#ifndef ADDRESS_H
#define ADDRESS_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef  MAX_PORT_NUMBER
#define MAX_PORT_NUMBER 65535
#endif

#ifndef BACKLOG
#define MAX_PORT_NUMBER 5
#endif

// Set up the address struct for the server socket
void configure_address(struct sockaddr_in* address, int port_num);

#endif //ADDRESS_H
