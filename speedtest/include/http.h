#ifndef SPEEDTEST_HTTP_H
#define SPEEDTEST_HTTP_H

// Start HTTP server on given port. Additional runtime parameters are passed so
// the HTTP layer can trigger tests without global variables.
// server: iperf3 host:port or identifier; duration: test duration seconds.
int http_server_start(int port, const char *server, int duration_seconds);
void http_server_stop();

#endif
