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

static int env_int(const char *name, int def){
    const char *v = getenv(name); if(!v||!*v) return def; return atoi(v);
}
static const char *env_str(const char *name, const char *def){
    const char *v = getenv(name); if(!v||!*v) return def; return v;
}

static int g_interval = 0; // seconds (future use for scheduled tests)
static int g_duration = 10;
static char g_server[128];

int main(int argc, char **argv){
    // Priority order for config: Environment (USP sets) > CLI args > defaults.
    int port = 8080;
    const char *server = "iperf3.example.net:5201";

    if (argc > 1) port = atoi(argv[1]);
    if (argc > 2) server = argv[2];

    port = env_int("SPEEDTEST_PORT", port);
    server = env_str("SPEEDTEST_SERVER", server);
    g_duration = env_int("SPEEDTEST_DURATION", 10);
    g_interval = env_int("SPEEDTEST_INTERVAL", 0); // 0 = disabled
    int run_on_start = env_int("SPEEDTEST_RUN_ON_START", 0);
    snprintf(g_server, sizeof(g_server), "%s", server);

    signal(SIGINT, handle_sig);
    signal(SIGTERM, handle_sig);

    printf("speedtestd starting on port %d using server %s (duration=%ds interval=%ds)\n", port, server, g_duration, g_interval);
    if (http_server_start(port, g_server, g_duration) != 0){
        fprintf(stderr, "Failed to start HTTP server\n");
        return 1;
    }

    if (run_on_start){
        // Fire an initial test by hitting the run function indirectly via server thread: simplest is to curl ourselves later.
        // Placeholder: future direct invocation hook could be added.
        printf("Configured to run a test immediately on startup (deferred until first API call or scheduler).\n");
    }

    (void)g_interval; // silence unused until scheduling implemented

    while (atomic_load(&running)){
        sleep(1);
    }

    http_server_stop();
    printf("speedtestd shutting down.\n");
    return 0;
}
