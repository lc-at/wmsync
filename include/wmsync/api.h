#include <wmsync/common.h>

#ifndef WMSYNC__API_H
int init_wsa();
unsigned long get_ip_by_mac(uint8_t mac_p[6], char *res_buf,
                            unsigned long res_buf_sz);
unsigned long get_best_if(unsigned long *index_p);
int mac_aton(const char *mac, uint8_t *buf);
int mac_ntoa(uint8_t *mac, char *buf);
#endif // WMSYNC__API_H