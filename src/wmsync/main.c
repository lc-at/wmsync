#include <inih/ini.h>
#include "common.h"

static int handler(void *user, const char *section, const char *name,
                   const char *value);
static void print_banner();

int main(int argc, char *argv[])
{
    configuration config;

    print_banner();

    if (argc != 2)
    {
        printf("usage: %s <config_path>\n", argv[0]);
        return 1;
    }

    if (ini_parse(argv[1], handler, &config) < 0)
    {
        printf("Can't load '%s'\n", argv[1]);
        return 1;
    }

    start_sync(&config);
    return 0;
}

static int handler(void *user, const char *section, const char *name,
                   const char *value)
{
    configuration *p_config = (configuration *)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("auth", "username"))
    {
        p_config->username = strdup(value);
    }
    else if (MATCH("auth", "password"))
    {
        p_config->password = strdup(value);
    }
    else if (MATCH("wms", "gw_id"))
    {
        p_config->gw_id = strdup(value);
    }
    else if (MATCH("wms", "wlan"))
    {
        p_config->wlan = strdup(value);
    }
    else if (MATCH("wms", "mac"))
    {
        p_config->gw_id = strdup(value);
    }
    else
    {
        return 0;
    }
    return 1;
}

static void print_banner() {
    printf("wmsync \u2014 WMS connection synchronizer\n");
}