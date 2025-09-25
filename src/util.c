#include "util.h"
#include <time.h>
#include <stdio.h>

void iso8601_time(char *buf, int buflen, long epoch){
    time_t t = (time_t)epoch;
    struct tm tm;
    gmtime_r(&t, &tm);
    strftime(buf, buflen, "%Y-%m-%dT%H:%M:%SZ", &tm);
}
