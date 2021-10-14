#include <wmsync/api.h>
#include <wmsync/common.h>

unsigned long get_netinfo(uint8_t *mac);

int start_sync(configuration *p_config) {
    int wsa_init_rv = 0;

    log_info("Initializing synchronizer");
    wsa_init_rv = init_wsa();
    if (wsa_init_rv != 0) {
        log_error("Failed to initialize Winsock (code: %d)", wsa_init_rv);
        exit(1);
    }

    log_info("Checking connection");
    get_netinfo(p_config->mac);
    return 0;
}

unsigned long get_netinfo(uint8_t *mac) {
    char ip_addr[16];
    unsigned long retval;
    char mac_addr[20];
    mac_ntoa(mac, mac_addr);
    log_info("MAC Address: %s", mac_addr);
    retval = get_ip_by_mac(mac, ip_addr, 16);
    if (retval != 0) {
        log_error("Failed to find IP address (code: %d)", retval);
    } else {
        log_info("IP Address: %s", ip_addr);
    }
    return retval;
}
