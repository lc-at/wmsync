#include <wmsync/api.h>
#include <wmsync/common.h>
#include <windows.h>

int start_sync(configuration *p_config) {
    int wsa_init_rv = 0;
    int mac_conv_rv = 0;
    uint8_t mac_addr_buf[6];

    mac_conv_rv = mac_aton(p_config->mac, mac_addr_buf);
    if (mac_conv_rv != 0) {
        log_error("Invalid MAC address. Check your config file.");
        exit(1);
    }

    log_info("Initializing synchronizer");
    wsa_init_rv = init_wsa();
    if (wsa_init_rv != 0) {
        log_error("Failed to initialize Winsock (code: %d)", wsa_init_rv);
        exit(1);
    }

    log_info("Starting login loop");
    while (1) {
        char ip_addr_buf[IP_ADDR_BUF_SZ];
        unsigned long get_ip_retval =
            get_ip_by_mac(mac_addr_buf, ip_addr_buf, IP_ADDR_BUF_SZ);
        if (get_ip_retval != 0) {
            log_error("Failed to get IP address for the specified MAC address "
                      "(code: %d).",
                      get_ip_retval);
            Sleep(LOGIN_ERROR_TA_INTERVAL);
            continue;
        }
        log_info("Sending a login request for IP address: %s", ip_addr_buf);
        Sleep(LOGIN_CHECK_INTERVAL);
    }
    return 0;
}
