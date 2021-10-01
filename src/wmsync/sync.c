#define WIN32_LEAN_AND_MEAN

#include "common.h"
#include <winsock2.h>
#include <windows.h> // noqa
#include <iphlpapi.h>

#define WORKING_BUFFER_SIZE 15000

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Ws2_32.lib")

long get_ip_address();

int start_sync(configuration *p_config) {
    log_info("Initializing synchronizer");
    log_info("Checking connection");
    get_ip_address();
    return 0;
}

long get_ip_address() {
    IPAddr dest_addr = inet_addr("8.8.8.8");
    DWORD best_if_index;
    DWORD retval = GetBestInterface(dest_addr, &best_if_index);
    log_trace("%d", retval);
    log_trace("%d", best_if_index);
    return 0;
}
