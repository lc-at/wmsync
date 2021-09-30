#ifndef COMMON_H
#define COMMON_H

#include <log.c/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int version;
    const char *username;
    const char *password;
    const char *gw_id;
    const char *wlan;
    const char *mac;
} configuration;

int start_sync(configuration *config);

#endif  // COMMON_H