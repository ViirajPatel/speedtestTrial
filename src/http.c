#include "http.h"
#include "speedtest.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdatomic.h>

static int listen_fd = -1;
static pthread_t server_thread;
static atomic_int server_running = 0;
static speedtest_result_t last_result;
static char cfg_server[128];
static int cfg_duration = 10;
static pthread_mutex_t result_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *server_loop(void *arg){
    (void)arg;
    char buf[2048];
    while (atomic_load(&server_running)){
        struct sockaddr_storage cliaddr; socklen_t clilen = sizeof(cliaddr);
        int cfd = accept(listen_fd,(struct sockaddr*)&cliaddr,&clilen);
        if (cfd < 0){
            if (errno == EINTR) continue;
            if (!atomic_load(&server_running)) break;
            perror("accept");
            continue;
        }
        int n = read(cfd, buf, sizeof(buf)-1);
        if (n <= 0){ close(cfd); continue; }
        buf[n] = 0;
    int run_now = 0;
        if (strncmp(buf, "GET /api/run", 12) == 0){
            run_now = 1;
        } else if (strncmp(buf, "GET /api/latest", 15) == 0){
            // just return cached result
        }
        pthread_mutex_lock(&result_mutex);
        if (run_now){
            speedtest_result_t r;
            run_speedtest(&r, cfg_server[0]?cfg_server:"demo-server", cfg_duration);
            last_result = r;
        }
        speedtest_result_t copy = last_result;
        pthread_mutex_unlock(&result_mutex);
        char ts[64]; iso8601_time(ts, sizeof(ts), copy.timestamp);
        char body[512];
        snprintf(body, sizeof(body),
                 "{\n  \"server\": \"%s\",\n  \"download_mbps\": %.2f,\n  \"upload_mbps\": %.2f,\n  \"latency_ms\": %.2f,\n  \"jitter_ms\": %.2f,\n  \"packet_loss_pct\": %.2f,\n  \"timestamp\": \"%s\",\n  \"status\": \"%s\"\n}\n",
                 copy.server, copy.download_mbps, copy.upload_mbps, copy.latency_ms, copy.jitter_ms, copy.packet_loss_pct, ts, copy.status);
        char header[256];
        snprintf(header, sizeof(header),
                 "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n", strlen(body));
    ssize_t wn = write(cfd, header, strlen(header));
    if (wn < 0) { perror("write header"); }
    wn = write(cfd, body, strlen(body));
    if (wn < 0) { perror("write body"); }
        close(cfd);
    }
    return NULL;
}

int http_server_start(int port, const char *server, int duration){
    if (atomic_load(&server_running)) return 0;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0){ perror("socket"); return -1; }

    int yes = 1; setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in addr4; memset(&addr4,0,sizeof(addr4));
    addr4.sin_family = AF_INET; addr4.sin_addr.s_addr = htonl(INADDR_ANY); addr4.sin_port = htons(port);
    if (bind(listen_fd, (struct sockaddr*)&addr4, sizeof(addr4)) < 0){ perror("bind"); close(listen_fd); return -1; }
    if (listen(listen_fd, 16) < 0){ perror("listen"); close(listen_fd); return -1; }

    atomic_store(&server_running, 1);
    if (pthread_create(&server_thread, NULL, server_loop, NULL) != 0){
        perror("pthread_create");
        close(listen_fd);
        atomic_store(&server_running, 0);
        return -1;
    }

    // Initialize config & default result
    memset(cfg_server,0,sizeof(cfg_server));
    if (server) snprintf(cfg_server,sizeof(cfg_server),"%s",server);
    cfg_duration = duration>0?duration:10;
    // Initialize default result
    memset(&last_result,0,sizeof(last_result));
    snprintf(last_result.status,sizeof(last_result.status),"NO_TEST");
    return 0;
}

void http_server_stop(){
    if (!atomic_load(&server_running)) return;
    atomic_store(&server_running, 0);
    shutdown(listen_fd, SHUT_RDWR);
    close(listen_fd);
    pthread_join(server_thread, NULL);
}
