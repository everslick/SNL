//
// SNL udp listener
//

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include <snl/snl.h>

static char *key = NULL;
static int shutdown = 0;

static char *
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
   const char *info, *err;

   switch (skt->event_code) {
      case SNL_EVENT_ERROR:
         err = snl_error_string(skt->error_code);

         printf("client error: %i (%s)\n", skt->error_code, err);
      break;

      case SNL_EVENT_RECEIVE:
         info = ipaddr(skt->client_ip, skt->client_port);

         printf("received ");
         if (skt->data_length > 55) {
             printf("%i bytes\n", skt->data_length);
         } else {
             printf("%s", (char *)skt->data_buffer);
         }
         printf(" from: %s\n", info);
      break;
   }
}

int
main(int argc, char **argv) {
   unsigned short int port = 3000;
   snl_socket_t *server = NULL;

   for (int i=1; i<argc; i++) {
      if (!strcmp(argv[i], "-p") && (argc>i+1)) port = atoi(argv[i+1]);
      if (!strcmp(argv[i], "-k")) key  = argv[i+1];
      if (!strcmp(argv[i], "--help")) {
         puts("");
         puts("udplistener " VERSION " <clemens@1541.org>");
         puts("");
         puts("USAGE: udplistener [-p port] [-k key]");
         puts("");
         puts("\t-p ... listen on port <port> (default 3000)");
         puts("\t-k ... set cipher key to <key> (default none)");
         exit(0);
      }
   }

   snl_init();

   signal(SIGINT,  quit);
   signal(SIGQUIT, quit);
   signal(SIGHUP,  quit);

   printf("starting UDP listener on port %i.\n", port);

   server = snl_socket_new(SNL_PROTO_UDP, event_callback, NULL);

   snl_passphrase(server, key);

   if (snl_listen(server, port)) {
      printf("could not start listener, exiting.\n");

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
