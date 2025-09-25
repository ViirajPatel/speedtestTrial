#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#include "speedtest.h"
#include "http.h"
#include "util.h"

static atomic_int running = 1;

static void handle_sig(int sig){
    (void)sig;
    atomic_store(&running, 0);
}

int main(int argc, char **argv){
    int port = 8080;
    const char *server = "iperf3.example.net:5201";

    if (argc > 1) port = atoi(argv[1]);
    if (argc > 2) server = argv[2];

    signal(SIGINT, handle_sig);
    signal(SIGTERM, handle_sig);

    printf("speedtestd starting on port %d using server %s\n", port, server);
    if (http_server_start(port) != 0){
        fprintf(stderr, "Failed to start HTTP server\n");
        return 1;
    }

    while (atomic_load(&running)){
        sleep(1);
    }

    http_server_stop();
    printf("speedtestd shutting down.\n");
    return 0;
}
