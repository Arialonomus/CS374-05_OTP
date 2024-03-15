/* Assignment: CS 374 Assignment 05 - OTP
 * Program: keygen
 * Author: Jacob Barber
 * UID: 934561945
 *
 * This program generates a keyfile of n characters consisting of capital
 * letters and spaces, to be used by encoding and decoding servers.
 */

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

#define CHAR_MAX 90
#define CHAR_MIN 65
#define CHAR_OFFSET CHAR_MIN
#define RAND_RANGE (CHAR_MAX - CHAR_MIN) + 2
#define SPACE_VALUE RAND_RANGE - 1

int main (int argc, char* argv[]) {

	// Initialization
	if (argc != 2 || atoi(argv[1]) < 1)
		errx(EXIT_FAILURE, "only one argument must be provided, consisting of a non-negative integer");
	const size_t key_len = atoi(argv[1]);
	char key_buf[key_len];
	srand(time(NULL));

	// Generate a string of random letters and the space character
	for (int i = 0; i < key_len; ++i) {
		const int num = 0 + rand() % RAND_RANGE;
		key_buf[i] = num == SPACE_VALUE ? ' ' : num + CHAR_OFFSET;
	}

	// Append null terminator and print string
	key_buf[key_len] = '\0';
	fputs(key_buf, stdout);
	fflush(stdout);

	return EXIT_SUCCESS;
}