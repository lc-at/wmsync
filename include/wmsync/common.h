#ifndef WMSYNC__COMMON_H
#define WMSYNC__COMMON_H

#include <log.c/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    int version;
    const char *username;
    const char *password;
    const char *gw_id;
    const char *wlan;
    uint8_t mac[6];
} configuration;

int start_sync(configuration *config);

#endif // WMSYNC__COMMON_H