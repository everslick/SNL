//
// SNL test client
//

#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <time.h>

#include "snl/snl.h"

static int shortest = INT_MAX, longest = 0, sum = 0, shutdown = 0;

static char *load = "abcdefghijklmnopqrstuvwxyz!@#$%^&*()1234567890";

static struct timeval start;

static void
msleep(int ms) {
   struct timespec tm;

   tm.tv_sec = ms / 1000;
   tm.tv_nsec = ms % 1000 * 1000;

   nanosleep(&tm, NULL);
}

static int
now(void) {
   struct timeval tv;
                                                                                
   gettimeofday(&tv, NULL);

   return ((tv.tv_sec-start.tv_sec)*1000000+(tv.tv_usec-start.tv_usec));
}
                                                                                
static void
quit(int sig) {
   if (!shutdown) {
      shutdown = 1;
   }
}

void
event_callback(snl_socket_t *skt) {
   static int sequence = 1;
   int delay = now();

   switch (skt->event_code) {
      case SNL_EVENT_ERROR:
         if (skt->error_code == SNL_ERROR_CLOSED) {
            snl_disconnect(skt);
            printf("server closed connection\n");
            shutdown = 1;
         } else {
            printf("socket error: %i\n", skt->error_code);
         }
      break;

      case SNL_EVENT_RECEIVE:
         if (memcmp(skt->data_buffer, load, skt->data_length)) {
            printf(" [DEMAGED]");
         }

         printf("%i bytes: seq=%i time=%.3f ms\n",
            skt->data_length,
            sequence++,
            delay/1000.0);

         if (delay < shortest) shortest = delay;
         if (delay > longest)  longest  = delay;

         sum += delay;
      break;
   }
}

int
main(int argc, char **argv) {
   int i, size = 0, seq = 0, count = 10;
   int port = 3000, interval = 1000;
   float min, max, avg;
   snl_socket_t *skt;
   char *key = NULL;

   for (i=1; i<argc; i++) {
      if (!strcmp(argv[i], "-k")) key      = argv[i+1];
      if (!strcmp(argv[i], "-p")) port     = atoi(argv[i+1]);
      if (!strcmp(argv[i], "-c")) count    = atoi(argv[i+1]);
      if (!strcmp(argv[i], "-s")) size     = atoi(argv[i+1]);
      if (!strcmp(argv[i], "-i")) interval = atoi(argv[i+1]);
      if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
         puts("");
         puts("client " VERSION " <clemens@1541.org>");
         puts("");
         puts("USAGE: client [-i int] [-p port] [-t text] [-k key] [-c cnt]");
         puts("\t-p ... use port <port> for connections (default 3000)");
         puts("\t-k ... set cipher key to <key>");
         puts("\t-s ... size of payload");
         puts("\t-i ... packet interval in ms (default 1000)");
         puts("\t-c ... transmit <cnt> packets then exit (default 10)");
         puts("");
         exit(0);
      }
   }

   snl_init();

   signal(SIGINT,  quit);
   signal(SIGQUIT, quit);
   signal(SIGHUP,  quit);

   if (count == -1) count = INT_MAX;

   skt = snl_socket_new(SNL_PROTO_MSG, event_callback, NULL);

   snl_passphrase(skt, key);

   if (!(snl_connect(skt, "localhost", port)) > 0) {
      if (size) {
         if (!(load = (char *)malloc(size))) {
            printf("couldn't allocate payload buffer\n");
            return (-2);
         }
      } else {
         size = strlen(load);
      }

      while ((seq < count) && (!shutdown)) {
         msleep(interval);
         gettimeofday(&start, NULL);
         if (snl_send(skt, load, size)) {
            printf("error while sending data to server\n");
         }
         seq++;
      }

      sleep(1); // wait for answer packets to arrive

      min = shortest/1000.0;
      max = longest/1000.0;
      avg = (sum/seq)/1000.0;

      printf("\n--- statistics ---\n");
      printf("%i packets transmitted\n", seq);
      printf("%i bytes sent, %i bytes received\n", skt->xfer_sent, skt->xfer_rcvd);
      printf("round-trip min/avg/max = %.3f/%.3f/%.3f ms\n", min, avg, max);

      snl_disconnect(skt);
   } else {
      printf("can't connect to server\n");

      return (-3);
   }

   snl_socket_delete(skt);

   return (0);
}
