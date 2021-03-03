/*
 * (C) Copyright 2017
 * Stefano Babic, DENX Software Engineering, sbabic@denx.de.
 *
 * SPDX-License-Identifier:     GPL-2.0-or-later
 */

/*
 * This is a simple example how to send a command to
 * a SWUpdate's subprocess. It sends a "feedback"
 * to the suricatta module and waits for the answer.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "network_ipc.h"


static void usage(char *program) {
	fprintf(stdout, "%s [OPTIONS] COMMAND\n", program);
	fprintf(stdout, "\t aes <key> <ivt>\n");
	fprintf(stdout, "\t setversion <minversion> <maxversion> <current>\n");
	exit(0);
}

static bool check_ascii_char(const char *s) {
	int i;

	if (!s)
		return false;
	for (i = 0; i < strlen(s); i++) {
		if ((s[i] >= '0' && s[i] <= '9') || 
			(s[i] >= 'A' && s[i] <= 'F'))
			continue;
		return false;
	}

	return true;
}
/*
 * Simple example, it does nothing but calling the library
 */
int main(int argc, char *argv[]) {
	if (argc < 2)
		usage(argv[0]);

	if (!strcmp(argv[1], "aes")) {
		char *key, *ivt;
		if (argc != 4)
			usage(argv[0]);
		key = argv[2];
		ivt = argv[3];
		if (strlen(key) != 64 || strlen(ivt) != 32) {
			fprintf(stderr, "Wrong format for AES /IVT\n");
			usage(argv[0]);
		}
		if (!check_ascii_char(key) || !check_ascii_char(ivt)) {
			fprintf(stderr, "Wrong chars in keys\n");
			exit(1);
		}
		if (swupdate_set_aes(key, ivt)) {
			fprintf(stderr, "Error setting AES KEY\n");
			exit(1);
		}
		exit(0);
	}

	if (!strcmp(argv[1], "setversion")) {

		if (argc != 5)
			usage(argv[0]);

		if (swupdate_set_version_range(argv[2], argv[3], argv[4])) {
			fprintf(stderr, "Error IPC setting versions\n");
		}
	}
	
	exit(0);
}
