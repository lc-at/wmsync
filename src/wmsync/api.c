#define WIN32_LEAN_AND_MEAN

#include <iphlpapi.h>
#include <winsock2.h>

#define WORKING_BUFFER_SIZE 15000
#define MAX_GET_BEST_IF_ATTEMPTS 5

// #pragma comment(lib, "IPHLPAPI.lib")
// #pragma comment(lib, "Ws2_32.lib")

unsigned long get_best_if(unsigned long *index_p) {
    IPAddr dest_addr = inet_addr("8.8.8.8");
    DWORD retval = 0;
    int tries;
    do {
        retval = GetBestInterface(dest_addr, index_p);
        tries++;
    } while (retval != 0 && tries <= MAX_GET_BEST_IF_ATTEMPTS);
    return retval;
}