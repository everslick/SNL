//
// SNL udp sender
//

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include <snl/snl.h>

static char *load = (char *)"abcdefghijklmnopqrstuvwxyz!#$%^&*()_+<>?:@;";

static int shutdown = 0;

static void
quit(int sig) {
   if (!shutdown) {
      shutdown = 1;
   }
}

int
main(int argc, char **argv) {
   char *host = (char *)"localhost";
   unsigned short int port = 3000;
   snl_socket_t *skt = NULL;
   char *key = NULL;
   int flush = 0;
   int fast = 0;
   int size = 0;

   for (int i=1; i<argc; i++) {
      if (!strcmp(argv[i], "-h") && (argc>i+1)) host = argv[i+1];
      if (!strcmp(argv[i], "-p") && (argc>i+1)) port = atoi(argv[i+1]);
      if (!strcmp(argv[i], "-s") && (argc>i+1)) size = atoi(argv[i+1]);
      if (!strcmp(argv[i], "-b")) host = NULL;
      if (!strcmp(argv[i], "-t")) fast = 1;
      if (!strcmp(argv[i], "-f")) flush = 1;
      if (!strcmp(argv[i], "--help")) {
         puts("");
         puts("udpsender " VERSION " <clemens@1541.org>");
         puts("");
         puts("USAGE: udpsender [-h host] [-p port] [-s size]");
         puts("                 [-b] [-t] [-f]");
         puts("");
         puts("\t-h ... host name (default localhost)");
         puts("\t-p ... port number (default 3000)");
         puts("\t-b ... use broadcast address (default off)");
         puts("\t-t ... send a packet each timeslice (default off)");
         puts("\t-f ... send as fast as possible (default off)");
         puts("\t-s ... payload size");
         puts("");
         exit(0);
      }
   }

   snl_init();

   signal(SIGINT,  quit);
   signal(SIGQUIT, quit);
   signal(SIGHUP,  quit);

   skt = snl_socket_new(SNL_PROTO_UDP, NULL, NULL);

   snl_passphrase(skt, key);

   if (!(snl_connect(skt, host, port)) > 0) {
      if (size) {
         load = (char *)malloc(size);
         memset(load, '*', size);
         load[size - 1] = 0;
      } else {
         size = strlen(load);
      }

      while (!shutdown) {
         if (snl_send(skt, load, size)) {
            printf("error while sending data to listener\n");
         }

         if (flush) continue;

         if (fast) usleep(1); else sleep(1);
      }
   } else {
      puts("could not connect to server, exiting.");
      exit(-1);
   }

   snl_socket_delete(skt);

   return (0);
}
