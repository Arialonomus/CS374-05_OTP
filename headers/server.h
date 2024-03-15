/* This file contains functions and constants related to server processes
 * for the one-time pad system
 */

#ifndef OTP_SERVER_H
#define OTP_SERVER_H

#ifndef  MAX_PORT_NUMBER
#define MAX_PORT_NUMBER 65535
#endif

#ifndef QUEUE_LIMIT
#define QUEUE_LIMIT 5
#endif

#include "cryptography.h"

void handle_encryption(void);

#endif //OTP_SERVER_H
