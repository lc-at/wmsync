#include <wmsync/api.h>
#include <wmsync/common.h>

long get_ip_address();

int start_sync(configuration *p_config) {
    log_info("Initializing synchronizer");
    log_info("Checking connection");
    get_ip_address();
    return 0;
}

long get_ip_address() {
    unsigned long if_index;
    get_best_if(&if_index);
    
    log_trace("%d", if_index);
    return 0;
}

