//
// SNL test server
// Clemens Kirchgatterer <clemens@1541.org>
//

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include "snl/snl.h"

static int shutdown = 0;

static char *ipaddr(unsigned int host, unsigned short port) {
	unsigned char *ip = (unsigned char *)&host;
	static char buf[32];

	sprintf(buf, "%i.%i.%i.%i:%i", ip[3], ip[2], ip[1], ip[0], port);

	return (buf);
}

static void
quit(int sig) {
	if (!shutdown) {
		shutdown = 1;
	}
}

static void
event_callback(snl_socket_t *skt) {
	const char *msg = "<html>hello, world!</html>";
	char *info;

	switch (skt->event_code) {
		case SNL_EVENT_ERROR:
			if (skt->error_code == SNL_ERROR_CLOSED) {
				printf("client closed connection\n");
				snl_disconnect(skt);
			} else {
				printf("client error: %i (%s)\n",
					skt->error_code,
					snl_error_string(skt->error_code));
			}
		break;

		case SNL_EVENT_ACCEPT:
			info = ipaddr(skt->client_ip, skt->client_port);
			printf("client connected from: %s\n", info);

			snl_write(skt->client_fd, msg, strlen(msg));
			close(skt->client_fd);
		break;
	}
}

int main(int argc, char **argv) {
	snl_socket_t *server;
	int port = 8080;

	for (int i=1; i<argc; i++) {
		if (!strcmp(argv[i], "-p")) port = atoi(argv[i+1]);
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			puts("");
			puts("webserver " VERSION " <clemens@1541.org>");
			puts("");
			puts("USAGE: webserver [-p port]");
			puts("\t-p ... use port <port> for connections (default 8080)");
			puts("");
			exit(0);
		}
	}

	snl_init();

	signal(SIGINT,  quit);
	signal(SIGQUIT, quit);
	signal(SIGHUP,  quit);

	printf("starting webserver on port %i.\n", port);

	server = snl_socket_new(SNL_PROTO_RAW, event_callback, NULL);
	if (snl_listen(server, port)) {
		printf("could not start server, exiting.\n");

		return (1);
	}

	while (!shutdown) {
		usleep(1000);
	}

	sleep(1);

	snl_disconnect(server);
	snl_socket_delete(server);
	
	return (0);
}
