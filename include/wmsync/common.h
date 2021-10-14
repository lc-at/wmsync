#ifndef WMSYNC__COMMON_H
#define WMSYNC__COMMON_H

#define LOGIN_CHECK_INTERVAL 30 * 1000 // milliseconds
#define LOGIN_ERROR_TA_INTERVAL 5 * 1000 // milliseconds
#define IP_ADDR_BUF_SZ 16

#include <log.c/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    const char *username;
    const char *password;
    const char *gw_id;
    const char *wlan;
    const char *mac;
} configuration;

int start_sync(configuration *config);

#endif // WMSYNC__COMMON_H