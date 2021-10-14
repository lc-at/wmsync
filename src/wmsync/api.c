#define WIN32_LEAN_AND_MEAN

#include <wmsync/common.h>
#include <wmsync/errors.h>

#include <winsock2.h>
#include <mstcpip.h>
#include <iphlpapi.h>

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

int init_wsa() {
    int retval;
    WSADATA wsa_data;
    retval = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (retval != 0) {
        log_debug("WSAStartup failed");
    }
    return retval;
}

unsigned long get_best_if(unsigned long *index_p) {
    IPAddr dest_addr = inet_addr("8.8.8.8");
    DWORD retval = 0;
    int tries;
    do {
        retval = GetBestInterface(dest_addr, index_p);
        tries++;
    } while (retval != 0 && tries <= MAX_TRIES);
    return retval;
}

unsigned long get_ip_by_mac(uint8_t mac_p[6], char *res_buf,
                             unsigned long res_buf_sz) {
    DWORD retval = 0;
    ULONG buf_sz = WORKING_BUFFER_SIZE;
    PIP_ADAPTER_ADDRESSES addresss_p;
    int i = 0;
    bool found = 0;

    if (res_buf_sz < (unsigned long)16) {
        log_debug("Buffer too small");
        return WMSYNC_BUF_TOO_SMALL;
    }

    do {
        addresss_p = (IP_ADAPTER_ADDRESSES *)malloc(buf_sz);
        if (addresss_p == NULL) {
            log_debug("Memory allocation failed");
            return WMSYNC_MEM_ALLOC_FAILED;
        }

        retval = GetAdaptersAddresses(AF_INET,
                                      GAA_FLAG_SKIP_ANYCAST |
                                          GAA_FLAG_SKIP_MULTICAST |
                                          GAA_FLAG_SKIP_DNS_SERVER,
                                      NULL, addresss_p, &buf_sz);

        if (retval == ERROR_BUFFER_OVERFLOW) {
            free(addresss_p);
            addresss_p = NULL;
        }
        i++;
    } while ((retval == ERROR_BUFFER_OVERFLOW) && (i < MAX_TRIES));

    if (retval == NO_ERROR) {
        PIP_ADAPTER_ADDRESSES current_p = addresss_p;
        while (current_p) {
            if (current_p->PhysicalAddressLength == 6) {
                found = 1;
                for (i = 0; i < (int)current_p->PhysicalAddressLength; i++) {
                    if ((int)current_p->PhysicalAddress[i] != (int)mac_p[i]) {
                        found = 0;
                        break;
                    };
                }
                if (found) {
                    int conv_rv = 0;
                    conv_rv = WSAAddressToStringA(
                        current_p->FirstUnicastAddress->Address.lpSockaddr,
                        (DWORD)current_p->FirstUnicastAddress->Address
                            .iSockaddrLength,
                        NULL, (LPSTR)res_buf, &res_buf_sz);
                    if (conv_rv != 0) {
                        log_debug("WSAAddressToStringA failed (code: %d)",
                                  WSAGetLastError());
                        found = 0;
                    }
                    break;
                }
            }
            current_p = current_p->Next;
        }
    }

    if (addresss_p)
        free(addresss_p);

    return (retval != 0) ? retval : (unsigned long)(!found);
}

int mac_aton(const char *mac, uint8_t *buf) {
    unsigned int values[6];
    int i;

    if (sscanf(mac, "%x:%x:%x:%x:%x:%x%*c", &values[0], &values[1], &values[2],
               &values[3], &values[4], &values[5]) == 6) {
        for (i = 0; i < 6; i++)
            buf[i] = (uint8_t)values[i];

        return 0;
    }
    return 1;
}

int mac_ntoa(uint8_t *mac, char *buf) {
    if (sprintf(buf, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", mac[0], mac[1], mac[2],
                mac[3], mac[4], mac[5]) == 6) {
        return 0;
    }
    return 1;
}