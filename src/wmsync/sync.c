#include "common.h"
#include <netdb.h>

bool check_connection();

int start_sync(configuration *p_config) {
    log_info("initializing synchronizer");
    log_info("checking connection");
    check_connection();
    return 0;
}

bool check_connection() {
    struct hostent *h;
    h = gethostbyname("google.com");
    log_trace("%d", h->h_length);
    return 0;
}
