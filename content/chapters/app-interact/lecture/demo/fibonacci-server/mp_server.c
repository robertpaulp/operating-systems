// SPDX-License-Identifier: BSD-3-Clause

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utils/utils.h"
#include "utils/log/log.h"
#include "utils/sock/sock_util.h"

#include "./connection.h"

static void handle_in_new_process(int connectfd)
{
	pid_t pid;

	pid = fork();
	switch (pid) {
	case -1:
		ERR(pid != 0, "fork");
		break;
	case 0:		/* child process */
		daemon(1, 1);
		handle_connection(connectfd);
		exit(EXIT_SUCCESS);
		break;
	default:
		log_info("Created process with ID %d to handle connection.", pid);
		break;
	}

	close(connectfd);
}

static void run_server(int port)
{
	int listenfd;		/* server socket */
	int connectfd;		/* client communication socket */

	/* create server socket */
	listenfd = tcp_create_listener(port, DEFAULT_LISTEN_BACKLOG);
	DIE(listenfd < 0, "tcp_create_listener");

	while (1) {
		connectfd = accept_connection(listenfd);
		DIE(connectfd < 0, "accept_connection");
		handle_in_new_process(connectfd);
	}
}

int main(int argc, char **argv)
{
	int port;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	port = (int) strtol(argv[1], NULL, 10);
	DIE(errno == ERANGE, "strtol");

	if (port < 0 || port > 65535) {
		fprintf(stderr, "Invalid port %d\n", port);
		exit(EXIT_FAILURE);
	}

	log_info("Starting server on port %d", port);
	run_server(port);

	return 0;
}
