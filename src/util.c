#include "util.h"
#include <time.h>
#include <stdio.h>

void iso8601_time(char *buf, int buflen, long epoch){
    time_t t = (time_t)epoch;
    struct tm *ptm = gmtime(&t);
    if (!ptm) {
        if (buf && buflen > 0) buf[0] = '\0';
        return;
    }
    strftime(buf, buflen, "%Y-%m-%dT%H:%M:%SZ", ptm);
}
