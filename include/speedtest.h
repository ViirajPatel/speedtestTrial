#ifndef SPEEDTEST_H
#define SPEEDTEST_H

#include <stdint.h>
#include <time.h>

typedef struct {
    char server[128];
    double download_mbps;
    double upload_mbps;
    double latency_ms;
    double jitter_ms;
    double packet_loss_pct;
    time_t timestamp;
    char status[32];
} speedtest_result_t;

int run_speedtest(speedtest_result_t *out, const char *server, int duration_seconds);

#endif /* SPEEDTEST_H */
