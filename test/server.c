//
// SNL test server
//

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include "snl/snl.h"

static char *key = NULL;

static int packets = 0, shutdown = 0, xfer_sent = 0, xfer_rcvd = 0;

static const char *
ipaddr(unsigned int host, unsigned short port) {
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
   snl_socket_t *client;
   const char *info;

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

      case SNL_EVENT_RECEIVE:
         xfer_rcvd += skt->data_length;
         snl_send(skt, skt->data_buffer, skt->data_length);
         xfer_sent += skt->data_length;
         packets++;
      break;

      case SNL_EVENT_ACCEPT:
         info = ipaddr(skt->client_ip, skt->client_port);
         printf("client connected from: %s\n", info);

         client = snl_socket_new(SNL_PROTO_MSG, event_callback, NULL);
         snl_passphrase(client, key);
         client->file_descriptor = skt->client_fd;
         snl_accept(client);
      break;
   }
}

int
main(int argc, char **argv) {
   unsigned short int port = 3000;
   snl_socket_t *server = NULL;

   for (int i=1; i<argc; i++) {
      if (!strcmp(argv[i], "-p")) port = atoi(argv[i+1]);
      if (!strcmp(argv[i], "-k")) key  = argv[i+1];
      if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
         puts("");
         puts("server " VERSION " <clemens@1541.org>");
         puts("");
         puts("USAGE: server [-p port] [-k key]");
         puts("\t-p ... use port <port> for connections (default 3000)");
         puts("\t-k ... set cipher key to <key> (default none)");
         puts("");
         exit(0);
      }
   }

   snl_init();

   signal(SIGINT,  quit);
   signal(SIGQUIT, quit);
   signal(SIGHUP,  quit);

   printf("starting server on port %i.\n", port);

   server = snl_socket_new(SNL_PROTO_MSG, event_callback, NULL);
   if (snl_listen(server, port)) {
      printf("could not start server, exiting.\n");

      return (1);
   }

   while (!shutdown) {
      usleep(1000);
   }

   sleep(1);

   printf("\n--- statistics ---\n");
   printf("%i packets transmitted\n", packets);
   printf("%i bytes sent, %i bytes received\n", xfer_sent, xfer_rcvd);

   snl_disconnect(server);
   snl_socket_delete(server);
   
   return (0);
}
