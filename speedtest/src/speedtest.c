#include "speedtest.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// Placeholder speed test implementation. Real implementation would invoke iperf3.
int run_speedtest(speedtest_result_t *out, const char *server, int duration_seconds){
    if(!out) return -1;
    memset(out, 0, sizeof(*out));
    snprintf(out->server, sizeof(out->server), "%s", server ? server : "unknown");
    // Simulated values
    out->download_mbps = 123.45;
    out->upload_mbps = 45.67;
    out->latency_ms = 17.2;
    out->jitter_ms = 1.4;
    out->packet_loss_pct = 0.1;
    out->timestamp = time(NULL);
    snprintf(out->status, sizeof(out->status), "%s", "SUCCESS");
    (void)duration_seconds; // unused in stub
    return 0;
}
